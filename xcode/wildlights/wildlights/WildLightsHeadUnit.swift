import Foundation
import CoreBluetooth

/* Services & Characteristics UUIDs */
//D4DCxxxx-83CD-4027-A5F3-D9D172A33130
let WildLightsServiceUUIDString                = "D4DC7777-83CD-4027-A5F3-D9D172A33130"
let WildLightsServiceUUID                      = CBUUID(string: WildLightsServiceUUIDString)

let WildLightsStreamingSyncStateCharUUIDString = "D4DC7779-83CD-4027-A5F3-D9D172A33130"
let WildLightsStreamingCharUUIDString          = "D4DC7778-83CD-4027-A5F3-D9D172A33130"

let WildLightsStreamingCharUUID                = CBUUID(string: WildLightsStreamingCharUUIDString)
let WildLightsSyncStateCharUUID                = CBUUID(string: WildLightsStreamingSyncStateCharUUIDString)
let WildLightsCharacteristicUUIDS              = [WildLightsStreamingCharUUID, WildLightsSyncStateCharUUID]



class WildLightsHeadUnit : NSObject, CBPeripheralDelegate {
    static let WildLightsHeadUnitConnected = "kWildLightsHeadUnitConnected"
    static let WildLightsHeadUnitDisconnected = "kWildLightsHeadUnitDisconnected"
    static let HeadUnitKey = "HeadUnit"
    var peripheral: CBPeripheral?
    var wildLightsStreaming : WildLightsStreaming?
    var wildLightsService : CBService?
    
    init(initWithPeripheral peripheral: CBPeripheral) {
        super.init()
        self.wildLightsStreaming = nil
        self.peripheral = peripheral
        self.peripheral?.delegate = self
        self.wildLightsService = nil
    }
    
    deinit {
        self.reset()
    }
    
    func startDiscoveringServices() {
        self.peripheral?.discoverServices(nil)
    }
    
    func reset() {
        if peripheral != nil {
            peripheral = nil
        }
        
        self.sendHeadUnitDisconnectedNotification()
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
                wildLightsService = service as? CBService;
                peripheral.discoverCharacteristics(WildLightsCharacteristicUUIDS, forService: service as! CBService)
                self.sendHeadUnitConnectedNotification()
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
                if (wildLightsStreaming == nil) {
                    wildLightsStreaming = WildLightsStreaming(peripheral: peripheral, service: service)
                }
                wildLightsStreaming!.syncChar = (characteristic as! CBCharacteristic)
                peripheral.setNotifyValue(true, forCharacteristic: wildLightsStreaming?.syncChar)
                println ("Found characteristic \(characteristic)")
            case WildLightsStreamingCharUUID:
                if (wildLightsStreaming == nil) {
                    wildLightsStreaming = WildLightsStreaming(peripheral: peripheral, service: service)
                }
                wildLightsStreaming!.streamChar = (characteristic as! CBCharacteristic)
                println ("Found characteristic \(characteristic)")
            default:
                break
            }
        }
    }
    
    // Mark: - Private
    
    func sendHeadUnitConnectedNotification() {
        NSNotificationCenter.defaultCenter().postNotificationName(WildLightsHeadUnit.WildLightsHeadUnitConnected, object: nil, userInfo: [WildLightsHeadUnit.HeadUnitKey:self])
    }
    func sendHeadUnitDisconnectedNotification() {
        NSNotificationCenter.defaultCenter().postNotificationName(WildLightsHeadUnit.WildLightsHeadUnitDisconnected, object: nil, userInfo: [WildLightsHeadUnit.HeadUnitKey:self])
    }
}

extension WildLightsHeadUnit : CBPeripheralDelegate {
    func peripheral(peripheral: CBPeripheral!, didUpdateValueForCharacteristic characteristic: CBCharacteristic!, error: NSError!) {
        NSNotificationCenter.defaultCenter().postNotificationName(characteristic.UUID.UUIDString, object:nil, userInfo: nil);
    }
}