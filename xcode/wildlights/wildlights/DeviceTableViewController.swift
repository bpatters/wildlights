//
//  DeviceTableViewController.swift
//  wildlights
//
//  Created by Bret Patterson on 5/21/15.
//  Copyright (c) 2015 Bret Patterson. All rights reserved.
//

import UIKit
import CoreBluetooth

class DeviceTableViewController: UITableViewController {
    let connectedReuseIdentifier = "connectedReuseIdentifier"
    let discoveredReuseIdentifier = "discoveredReuseIdentifier"
    var connectedDevices : [NSUUID:WildLightsHeadUnit] = [:];
    var discoveredDevices : [NSUUID:CBPeripheral] = [:];
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        for unit in WLDeviceManager.sharedInstance.connectedDevices.values {
            connectedDevices[unit.peripheral.identifier] = unit
        }
    }
    
    override func viewDidAppear(animated: Bool) {

        NSNotificationCenter.defaultCenter().addObserverForName(WLDeviceManager.DiscoveredDevice, object:nil, queue: nil) { note in
            dispatch_async(dispatch_get_main_queue(),{
                var userInfo = note.userInfo as! Dictionary<String, CBPeripheral>
                var peripheral = userInfo[WLDeviceManager.DeviceKey]
                self.discoveredDevices[peripheral!.identifier] = peripheral
                self.tableView.reloadData()
            });

        }
        NSNotificationCenter.defaultCenter().addObserverForName(WildLightsHeadUnit.WildLightsHeadUnitConnected, object:nil, queue: nil) { note in
            dispatch_async(dispatch_get_main_queue(),{
                dispatch_async(dispatch_get_main_queue(),{
                    var userInfo = note.userInfo as! Dictionary<String, WildLightsHeadUnit>
                    var headUnit = userInfo[WLDeviceManager.DeviceKey]
                    self.connectedDevices[headUnit!.peripheral.identifier] = headUnit
                    self.discoveredDevices[headUnit!.peripheral.identifier] = nil
                    self.tableView.reloadData()
                });
            });
        }
        NSNotificationCenter.defaultCenter().addObserverForName(WildLightsHeadUnit.WildLightsHeadUnitDisconnected, object:nil, queue: nil) { note in
            dispatch_async(dispatch_get_main_queue(),{
                dispatch_async(dispatch_get_main_queue(),{
                    var userInfo = note.userInfo as! Dictionary<String, WildLightsHeadUnit>
                    var headUnit = userInfo[WLDeviceManager.DeviceKey]
                    self.connectedDevices[headUnit!.peripheral.identifier] = nil
                    self.discoveredDevices[headUnit!.peripheral.identifier] = headUnit?.peripheral
                    self.tableView.reloadData()
                });
            });
        }
        WLDeviceManager.sharedInstance.startScanning()
    }

    override func viewDidDisappear(animated: Bool) {
        WLDeviceManager.sharedInstance.stopScanning()
    }
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
    }

    // MARK: - Table view data source



    /*
    override func tableView(tableView: UITableView, cellForRowAtIndexPath indexPath: NSIndexPath) -> UITableViewCell {
        let cell = tableView.dequeueReusableCellWithIdentifier("reuseIdentifier", forIndexPath: indexPath) as! UITableViewCell

        // Configure the cell...

        return cell
    }
    */

    /*
    // Override to support conditional editing of the table view.
    override func tableView(tableView: UITableView, canEditRowAtIndexPath indexPath: NSIndexPath) -> Bool {
        // Return NO if you do not want the specified item to be editable.
        return true
    }
    */

    /*
    // Override to support editing the table view.
    override func tableView(tableView: UITableView, commitEditingStyle editingStyle: UITableViewCellEditingStyle, forRowAtIndexPath indexPath: NSIndexPath) {
        if editingStyle == .Delete {
            // Delete the row from the data source
            tableView.deleteRowsAtIndexPaths([indexPath], withRowAnimation: .Fade)
        } else if editingStyle == .Insert {
            // Create a new instance of the appropriate class, insert it into the array, and add a new row to the table view
        }    
    }
    */

    /*
    // Override to support rearranging the table view.
    override func tableView(tableView: UITableView, moveRowAtIndexPath fromIndexPath: NSIndexPath, toIndexPath: NSIndexPath) {

    }
    */

    /*
    // Override to support conditional rearranging of the table view.
    override func tableView(tableView: UITableView, canMoveRowAtIndexPath indexPath: NSIndexPath) -> Bool {
        // Return NO if you do not want the item to be re-orderable.
        return true
    }
    */

    /*
    // MARK: - Navigation

    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepareForSegue(segue: UIStoryboardSegue, sender: AnyObject?) {
        // Get the new view controller using [segue destinationViewController].
        // Pass the selected object to the new view controller.
    }
    */
}

extension DeviceTableViewController : UITableViewDataSource {

    
    override func numberOfSectionsInTableView(tableView: UITableView) -> Int {
        // #warning Potentially incomplete method implementation.
        // Return the number of sections.
        return 2
    }
    
    override func tableView(tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        // #warning Incomplete method implementation.
        // Return the number of rows in the section.
        if (section == 0) {
            return connectedDevices.count
        } else {
            return discoveredDevices.count
        }
    }
    override  func tableView(tableView: UITableView,
        titleForHeaderInSection section: Int) -> String? {
            if section == 0 {
                return "Connected Devices"
            } else {
                return "Discovered Devices"
            }
    }
    
    override func tableView(tableView: UITableView, cellForRowAtIndexPath indexPath: NSIndexPath) -> UITableViewCell {
        
        if (indexPath.section == 0) {
            var cell = tableView.dequeueReusableCellWithIdentifier(connectedReuseIdentifier, forIndexPath:indexPath) as! UITableViewCell
            var headUnit = connectedDevices.values.array[indexPath.row]
            cell.textLabel?.text = headUnit.name;
            if (headUnit.peripheral.state == CBPeripheralState.Connected) {
                cell.detailTextLabel!.text = "Connected"
            } else {
                cell.detailTextLabel!.text = ""
            }
            
            return cell
        } else {
            var cell = tableView.dequeueReusableCellWithIdentifier(discoveredReuseIdentifier, forIndexPath:indexPath) as! UITableViewCell
            var periph = discoveredDevices.values.array[indexPath.row]
            cell.textLabel?.text = periph.name;
            if (periph.state == CBPeripheralState.Connected) {
                cell.detailTextLabel!.text = "Connected"
            } else {
                cell.detailTextLabel!.text = ""
            }
            
            return cell;
        }
        
    }
}

extension DeviceTableViewController : UITableViewDelegate {
    override func tableView(tableView: UITableView, didSelectRowAtIndexPath indexPath: NSIndexPath) {
        if (indexPath.section == 0 && connectedDevices.count > indexPath.row) {
            WLDeviceManager.sharedInstance.disconnectFrom(connectedDevices.values.array[indexPath.row])
        } else if (indexPath.section == 1 && discoveredDevices.count > indexPath.row) {
            WLDeviceManager.sharedInstance.connectTo(discoveredDevices.values.array[indexPath.row])
        }
    }
}
