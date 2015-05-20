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
    private let deviceReuseIdentifier   = "DeviceCell"
    private let programReuseIdentifier  = "ProgramCell"
    private let sectionInsets           = UIEdgeInsets(top: 50.0, left: 20.0, bottom: 50.0, right: 20.0)
    private var headUnits : [WildLightsHeadUnit] = []
    private var programs : [LedProgram] = []
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        // Uncomment the following line to preserve selection between presentations
        // self.clearsSelectionOnViewWillAppear = false
        
        // Register cell classes
        self.collectionView!.registerClass(UICollectionViewCell.self, forCellWithReuseIdentifier: reuseIdentifier)
        btDiscoverySharedInstance.startScanning()
        // Do any additional setup after loading the view.
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
        return headUnits.count + programs.count
    }
    
    //3
    override func collectionView(collectionView: UICollectionView, cellForItemAtIndexPath indexPath: NSIndexPath) -> UICollectionViewCell {
        if (indexPath.row <= headUnits.count) {
            let cell = collectionView.dequeueReusableCellWithReuseIdentifier(deviceReuseIdentifier, forIndexPath: indexPath) as! DeviceCollectionViewCell
            cell.backgroundColor = UIColor.blackColor()
            cell.labelView!.text = self.headUnits[indexPath.row].description
            // Configure the cell
            return cell
        } else {
            let cell = collectionView.dequeueReusableCellWithReuseIdentifier(programReuseIdentifier, forIndexPath: indexPath) as! UICollectionViewCell
            cell.backgroundColor = UIColor.blackColor()
            // Configure the cell
            return cell
        }
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