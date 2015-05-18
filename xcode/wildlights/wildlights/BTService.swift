import Foundation
import CoreBluetooth

/* Services & Characteristics UUIDs */
//D4DCxxxx-83CD-4027-A5F3-D9D172A33130
let LEDStreamServiceUUIDString       = "D4DC7777-83CD-4027-A5F3-D9D172A33130"
let LEDStreamSyncStateCharUUIDString =  "D4DC7779-83CD-4027-A5F3-D9D172A33130"
let LEDStreamCharUUIDString          =  "D4DC7778-83CD-4027-A5F3-D9D172A33130"
let LEDStreamServiceUUID             = CBUUID(string: LEDStreamServiceUUIDString)
let LEDStreamCharUUID                = CBUUID(string: LEDStreamCharUUIDString)
let LEDStreamSyncStateCharUUID       = CBUUID(string: LEDStreamSyncStateCharUUIDString)

let BLEServiceChangedStatusNotification = "kBLEServiceChangedStatusNotification"


class BTService: NSObject, CBPeripheralDelegate {
    var peripheral: CBPeripheral?
    var ledStreamService : LEDStreamService?
    
    init(initWithPeripheral peripheral: CBPeripheral) {
        super.init()
        self.ledStreamService = nil
        self.peripheral = peripheral
        self.peripheral?.delegate = self
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
        
        self.sendBTServiceNotificationWithIsBluetoothConnected(false)
    }
    
    // Mark: - CBPeripheralDelegate
    
    func peripheral(peripheral: CBPeripheral!, didDiscoverServices error: NSError!) {
        let uuidsForBTService: [CBUUID] = [LEDStreamCharUUID]
        
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
            if service.UUID == LEDStreamServiceUUID {
                ledStreamService = LEDStreamService(peripheral: peripheral, service: service as! CBService)
                peripheral.discoverCharacteristics([LEDStreamCharUUID], forService: service as! CBService)
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
            case LEDStreamSyncStateCharUUID:
                ledStreamService!.syncChar = (characteristic as! CBCharacteristic)
                println ("Found characteristic \(characteristic)")
            case LEDStreamCharUUID:
                ledStreamService!.streamChar = (characteristic as! CBCharacteristic)
                println ("Found characteristic \(characteristic)")
                self.sendBTServiceNotificationWithIsBluetoothConnected(true)
            default:
                break
            }
        }
    }
    
    // Mark: - Private
    
    func writeLedStrip(ledStrip: LedStrip) {
    }
    
    func sendBTServiceNotificationWithIsBluetoothConnected(isBluetoothConnected: Bool) {
        let connectionDetails = ["isConnected": isBluetoothConnected]
        NSNotificationCenter.defaultCenter().postNotificationName(BLEServiceChangedStatusNotification, object: nil, userInfo: connectionDetails)
    }
}

extension BTService : CBPeripheralDelegate {
    func peripheral(peripheral: CBPeripheral!, didUpdateValueForCharacteristic characteristic: CBCharacteristic!, error: NSError!) {
        NSNotificationCenter.defaultCenter().postNotificationName(characteristic.UUID.UUIDString, object:nil, userInfo: nil);
    }
}