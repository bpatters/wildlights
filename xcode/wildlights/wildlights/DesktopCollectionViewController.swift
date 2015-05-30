//
//  DesktopCollectionViewController.swift
//  wildlights
//
//  Created by Bret Patterson on 5/18/15.
//  Copyright (c) 2015 Bret Patterson. All rights reserved.
//

import UIKit

let reuseIdentifier = "Cell"

class DesktopCollectionViewController: UICollectionViewController {
    private let itemReuseIdentifier   = "ItemCell"
    private let sectionInsets           = UIEdgeInsets(top: 50.0, left: 20.0, bottom: 50.0, right: 20.0)
    private var items : [AnyObject] = []
    @IBOutlet weak var addDevice : UIBarButtonItem?
    var selectedDevice : WildLightsHeadUnit?
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        // Uncomment the following line to preserve selection between presentations
        // self.clearsSelectionOnViewWillAppear = false
        
        // Register cell classes
        self.collectionView!.registerClass(UICollectionViewCell.self, forCellWithReuseIdentifier: reuseIdentifier)
        // Do any additional setup after loading the view.
        for (uuid, headUnit) in WLDeviceManager.sharedInstance.connectedDevices {
            items.append(headUnit)
        }
        // handle reconnects
        NSNotificationCenter.defaultCenter().addObserverForName(WildLightsHeadUnit.WildLightsHeadUnitConnected, object:nil, queue: nil) { note in
            dispatch_async(dispatch_get_main_queue(),{
                var userInfo = note.userInfo as! Dictionary<String, WildLightsHeadUnit>
                var headUnit = userInfo[WLDeviceManager.DeviceKey]
                self.items.append(headUnit!)
                self.collectionView?.reloadData()
            });
        }
        // handle disconnections
        NSNotificationCenter.defaultCenter().addObserverForName(WildLightsHeadUnit.WildLightsHeadUnitDisconnected, object:nil, queue: nil) { note in
            dispatch_async(dispatch_get_main_queue(),{
                var userInfo = note.userInfo as! Dictionary<String, WildLightsHeadUnit>
                var headUnit = userInfo[WLDeviceManager.DeviceKey]
                for (index, item) in enumerate(self.items) {
                    if (item is WildLightsHeadUnit) {
                        var wl = item as! WildLightsHeadUnit
                        if (wl.identifier == headUnit?.identifier) {
                            self.items.removeAtIndex(index)
                            break;
                        }
                    }
                }
                self.collectionView?.reloadData()
            });
        }
    }
    
    
}

extension DesktopCollectionViewController : UITextFieldDelegate {
    func textFieldShouldReturn(textField: UITextField) -> Bool {
        // 1
        let activityIndicator = UIActivityIndicatorView(activityIndicatorStyle: .Gray)
        textField.addSubview(activityIndicator)
        activityIndicator.frame = textField.bounds
        
        textField.text = nil
        textField.resignFirstResponder()
        return true
    }
    
    
}

extension DesktopCollectionViewController : UICollectionViewDataSource {
    
    //1
    override func numberOfSectionsInCollectionView(collectionView: UICollectionView) -> Int {
        return 1
    }
    
    //2
    override func collectionView(collectionView: UICollectionView, numberOfItemsInSection section: Int) -> Int {
        return items.count
    }
    
    //3
    override func collectionView(collectionView: UICollectionView, cellForItemAtIndexPath indexPath: NSIndexPath) -> UICollectionViewCell {
        var cell = collectionView.dequeueReusableCellWithReuseIdentifier(itemReuseIdentifier, forIndexPath: indexPath) as! ItemCollectionViewCell
        cell.backgroundColor = UIColor.blackColor()
        var item : AnyObject = self.items[indexPath.row]
        if (item is WildLightsHeadUnit) {
            cell.nameLabel!.text = (item as? WildLightsHeadUnit)?.name
        } else if (item is LedProgram) {
            cell.nameLabel!.text = (item as? LedProgram)?.name
        }
        
        return cell
        
    }
}

extension DesktopCollectionViewController : UICollectionViewDelegateFlowLayout {
    //1
    func collectionView(collectionView: UICollectionView,
        layout collectionViewLayout: UICollectionViewLayout,
        sizeForItemAtIndexPath indexPath: NSIndexPath) -> CGSize {
            return CGSize(width: 128, height: 128)
    }
    
    //3
    func collectionView(collectionView: UICollectionView,
        layout collectionViewLayout: UICollectionViewLayout,
        insetForSectionAtIndex section: Int) -> UIEdgeInsets {
            return sectionInsets
    }
}

extension DesktopCollectionViewController : UICollectionViewDelegate {
    override func collectionView(collectionView: UICollectionView, didSelectItemAtIndexPath indexPath: NSIndexPath) {
        self.selectedDevice = items[indexPath.row] as? WildLightsHeadUnit
        self.performSegueWithIdentifier("deviceSettingsSegue", sender: self)
    }
    
    override func prepareForSegue(segue: UIStoryboardSegue, sender: AnyObject?) {
        if (segue.identifier == "deviceSettingsSegue") {
            var controller = segue.destinationViewController as! DeviceSettingsViewController
            
            controller.device = self.selectedDevice
        }
    }
}