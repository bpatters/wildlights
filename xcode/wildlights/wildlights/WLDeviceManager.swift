import Foundation
import CoreBluetooth



class WLDeviceManager: NSObject, CBCentralManagerDelegate {
    static let sharedInstance = WLDeviceManager();
    
    static let DiscoveredDevice = "kDiscoveredDevice"
    static let DeviceKey = "DeviceKey"
    private var centralManager: CBCentralManager?
    var connectedDevices: [NSUUID: WildLightsHeadUnit] = [:]
    var discovered: [NSUUID: CBPeripheral] = [:]
    
    override init() {
        super.init()
        
        let centralQueue = dispatch_queue_create("com.patterson", DISPATCH_QUEUE_SERIAL)
        centralManager = CBCentralManager(delegate: self, queue: centralQueue)
    }
    
    func startScanning() {
        discovered.removeAll(keepCapacity: false)
        if let central = centralManager {
            central.scanForPeripheralsWithServices(nil, options: [CBCentralManagerScanOptionAllowDuplicatesKey: false])
        }
    }
    
    func stopScanning() {
        if let central = centralManager {
            central.stopScan()
        }
    }
    
    // MARK: - CBCentralManagerDelegate
    
    func centralManager(central: CBCentralManager!, didDiscoverPeripheral peripheral: CBPeripheral!, advertisementData: [NSObject : AnyObject]!, RSSI: NSNumber!) {
        
        // Validate peripheral information
        if ((peripheral == nil) || (peripheral.name == nil) || (peripheral.name != "WildLights")) {
            return
        }
        
        // If not already connected to a peripheral, then connect to this one
        if (self.connectedDevices[peripheral.identifier] == nil && discovered[peripheral.identifier] == nil) {
            discovered[peripheral.identifier] = peripheral
            NSNotificationCenter.defaultCenter().postNotificationName(WLDeviceManager.DiscoveredDevice, object: nil, userInfo:[WLDeviceManager.DeviceKey: peripheral])
        }

    }
    
    func connectTo(peripheral: CBPeripheral) {
        let deviceInfo  = self.connectedDevices[peripheral.identifier]
        if (deviceInfo == nil || (deviceInfo!.peripheral.state == CBPeripheralState.Disconnected)) {
            // Connect to peripheral
            self.centralManager!.connectPeripheral(peripheral, options: nil)
        }
    }
    
    func disconnectFrom(headUnit: WildLightsHeadUnit) {
        let deviceInfo  = self.connectedDevices[headUnit.peripheral.identifier]
        if (deviceInfo != nil || (deviceInfo!.peripheral.state == CBPeripheralState.Connected)) {
            // Connect to peripheral
            self.connectedDevices[headUnit.peripheral.identifier] = nil
            centralManager?.cancelPeripheralConnection(headUnit.peripheral)
            sendHeadUnitDisconnectedNotification(headUnit)
        }
    }
    
    func centralManager(central: CBCentralManager!, didConnectPeripheral peripheral: CBPeripheral!) {
        
        if (peripheral == nil) {
            return;
        }
        
        // Create new service class
        let deviceInfo  = self.connectedDevices[peripheral.identifier]
        if (deviceInfo == nil) {
            var headUnit = WildLightsHeadUnit(initWithPeripheral: peripheral)
            
            connectedDevices[peripheral.identifier] = headUnit // add to connected list
            discovered[peripheral.identifier] = nil // remove from discovered list
            headUnit.startDiscoveringServices()
            sendHeadUnitConnectedNotification(headUnit)
        }
    }
    
    func centralManager(central: CBCentralManager!, didDisconnectPeripheral peripheral: CBPeripheral!, error: NSError!) {
        
        if (peripheral == nil) {
            return;
        }
        
        if (connectedDevices[peripheral.identifier] != nil) {
            sendHeadUnitDisconnectedNotification(connectedDevices[peripheral.identifier]!)
            connectedDevices[peripheral.identifier] = nil
            
        }
        if (discovered[peripheral.identifier] != nil) {
            discovered[peripheral.identifier] = nil
        }
    }
    
    // MARK: - Private
    
    func clearDevices() {
        connectedDevices.removeAll(keepCapacity: true)
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
    
    func sendHeadUnitConnectedNotification(headUnit: WildLightsHeadUnit) {
        NSNotificationCenter.defaultCenter().postNotificationName(WildLightsHeadUnit.WildLightsHeadUnitConnected, object: nil, userInfo: [WLDeviceManager.DeviceKey:headUnit])
    }
    func sendHeadUnitDisconnectedNotification(headUnit: WildLightsHeadUnit) {
        NSNotificationCenter.defaultCenter().postNotificationName(WildLightsHeadUnit.WildLightsHeadUnitDisconnected, object: nil, userInfo: [WLDeviceManager.DeviceKey:headUnit])
    }
}
