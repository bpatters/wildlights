import Foundation
import CoreBluetooth

let btDiscoverySharedInstance = BTDiscovery();

class BTDiscovery: NSObject, CBCentralManagerDelegate {
    
    private var centralManager: CBCentralManager?
    private var bleDevices: [NSUUID: (CBPeripheral, WildLightsHeadUnit?)] = [:]
    
    override init() {
        super.init()
        
        let centralQueue = dispatch_queue_create("com.patterson", DISPATCH_QUEUE_SERIAL)
        centralManager = CBCentralManager(delegate: self, queue: centralQueue)
    }
    
    func startScanning() {
        if let central = centralManager {
            central.scanForPeripheralsWithServices(nil, options: [CBCentralManagerScanOptionAllowDuplicatesKey: false])
        }
    }
    
    // MARK: - CBCentralManagerDelegate
    
    func centralManager(central: CBCentralManager!, didDiscoverPeripheral peripheral: CBPeripheral!, advertisementData: [NSObject : AnyObject]!, RSSI: NSNumber!) {
        
        // Validate peripheral information
        if ((peripheral == nil) || (peripheral.name == nil) || (peripheral.name != "WildLights")) {
            return
        }
        

        // If not already connected to a peripheral, then connect to this one
        let deviceInfo  = self.bleDevices[peripheral.identifier]
        if (deviceInfo == nil || (deviceInfo!.0.state == CBPeripheralState.Disconnected)) {
             bleDevices[peripheral.identifier] = (peripheral, nil)
            // Connect to peripheral
             central.connectPeripheral(peripheral, options: nil)
        }
    }
    
    func centralManager(central: CBCentralManager!, didConnectPeripheral peripheral: CBPeripheral!) {
        
        if (peripheral == nil) {
            return;
        }
        
        // Create new service class
        let deviceInfo  = self.bleDevices[peripheral.identifier]
        if (deviceInfo != nil) {
            let (device, service) = deviceInfo!
            var btService = WildLightsHeadUnit(initWithPeripheral: peripheral)
            
            bleDevices[peripheral.identifier] = (peripheral, btService)
            
            btService.startDiscoveringServices()
        }
    }
    
    func centralManager(central: CBCentralManager!, didDisconnectPeripheral peripheral: CBPeripheral!, error: NSError!) {
        
        if (peripheral == nil) {
            return;
        }
        
        if (bleDevices[peripheral.identifier] != nil) {
            bleDevices[peripheral.identifier] = nil
        }
    }
    
    // MARK: - Private
    
    func clearDevices() {
        bleDevices.removeAll(keepCapacity: true)
    }
    
    func centralManagerDidUpdateState(central: CBCentralManager!) {
        switch (central.state) {
        case CBCentralManagerState.PoweredOff:
            self.clearDevices()
            
        case CBCentralManagerState.Unauthorized:
            // Indicate to user that the iOS device does not support BLE.
            break
            
        case CBCentralManagerState.Unknown:
            // Wait for another event
            break
            
        case CBCentralManagerState.PoweredOn:
            self.startScanning()
            
        case CBCentralManagerState.Resetting:
            self.clearDevices()
            
        case CBCentralManagerState.Unsupported:
            break
            
        default:
            break
        }
    }
    
}
