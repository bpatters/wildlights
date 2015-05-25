import Foundation
import CoreBluetooth

/* Services & Characteristics UUIDs */
//D4DCxxxx-83CD-4027-A5F3-D9D172A33130
let WildLightsServiceUUIDString                = "D4DC7777-83CD-4027-A5F3-D9D172A33130"
let WildLightsServiceUUID                      = CBUUID(string: WildLightsServiceUUIDString)


let WildLightsStreamingCharUUIDString          = "D4DC7778-83CD-4027-A5F3-D9D172A33130"
let WildLightsStreamingSyncStateCharUUIDString = "D4DC7779-83CD-4027-A5F3-D9D172A33130"
let WildLightsProgramCharUUIDString            = "D4DC7780-83CD-4027-A5F3-D9D172A33130"

let WildLightsStreamingCharUUID                = CBUUID(string: WildLightsStreamingCharUUIDString)
let WildLightsSyncStateCharUUID                = CBUUID(string: WildLightsStreamingSyncStateCharUUIDString)
let WildLightsProgramCharUUID                  = CBUUID(string: WildLightsProgramCharUUIDString)
let WildLightsCharacteristicUUIDS              = [WildLightsStreamingCharUUID, WildLightsSyncStateCharUUID, WildLightsProgramCharUUID]


var headUnitsSharedInstance : [String:WildLightsHeadUnit] = [:]

class WildLightsHeadUnit : NSObject, CBPeripheralDelegate {
    static let WildLightsHeadUnitConnected          = "kWildLightsHeadUnitConnected"
    static let WildLightsHeadUnitDisconnected       = "kWildLightsHeadUnitDisconnected"
    static let HeadUnitKey                          = "HeadUnit"
    var peripheral          : CBPeripheral
    var wlService           : CBService?
    var wlStreamChar        : CBCharacteristic?
    var wlSyncChar          : CBCharacteristic?
    var wlDeviceSettings    : DeviceSettings

    var streamReady         : Bool                  = true;
    
    var identifier          : String { get { return wlDeviceSettings.identifier } }
    var name                : String { get { return wlDeviceSettings.name } set { wlDeviceSettings.name = newValue} }
    var strip               : LedStrip { get { return wlDeviceSettings.program.strips[0] } }
    var program             : LedProgram { get { return wlDeviceSettings.program } }
    
    init(initWithPeripheral peripheral: CBPeripheral) {
        self.wlStreamChar = nil
        self.wlSyncChar   = nil
        self.peripheral = peripheral
        self.wlService = nil
        self.wlDeviceSettings = DeviceSettings(name: peripheral.name, identifier: peripheral.identifier.UUIDString)
        
        super.init()
        
        self.peripheral.delegate = self
    }

    
    func startDiscoveringServices() {
        self.peripheral.discoverServices(nil)
    }
    // Mark: - CBPeripheralDelegate
    
    func peripheral(peripheral: CBPeripheral!, didDiscoverServices error: NSError!) {
        let uuidsForBTService: [CBUUID] = [WildLightsServiceUUID]
        
        if (peripheral != self.peripheral) {
            return
        }
        
        if (error != nil) {
            return
        }
        
        if ((peripheral.services == nil) || (peripheral.services.count == 0)) {
            return
        }
        
        for service in peripheral.services {
            if service.UUID == WildLightsServiceUUID {
                self.wlService = service as? CBService;
                peripheral.discoverCharacteristics(WildLightsCharacteristicUUIDS, forService: service as! CBService)
                headUnitsSharedInstance[self.identifier] = self
            }
        }
    }
    
    func peripheral(peripheral: CBPeripheral!, didDiscoverCharacteristicsForService service: CBService!, error: NSError!) {
        if (peripheral != self.peripheral) {
            return
        }
        
        if (error != nil) {
            return
        }
        
        for characteristic in service.characteristics {
            switch (characteristic.UUID) {
            case WildLightsSyncStateCharUUID:
                self.wlSyncChar = characteristic as? CBCharacteristic
                peripheral.setNotifyValue(true, forCharacteristic: wlSyncChar)
                println ("Found characteristic \(characteristic)")
            case WildLightsStreamingCharUUID:
                self.wlStreamChar = characteristic as? CBCharacteristic
                println ("Found characteristic \(characteristic)")
            default:
                break
            }
        }
    }
    
    func peripheral(peripheral: CBPeripheral!, didWriteValueForCharacteristic characteristic: CBCharacteristic!, error: NSError!) {
            if (characteristic == self.wlStreamChar) {
                self.streamReady = true
            }
    }
}

extension WildLightsHeadUnit {

    func sendMiniProgram(program: LEDMiniProgram) {
        assert(streamReady, "Must wait for previous stream send to complete before issuing another")
        
        streamReady = false;
        writeStreamData(program.getDataPacket())
    }
    
    private func writeStreamData(data: NSData) {
        self.peripheral.writeValue(data, forCharacteristic: wlStreamChar!, type: .WithResponse)
        println ("sending packet \(data) to device")
    }
    
}