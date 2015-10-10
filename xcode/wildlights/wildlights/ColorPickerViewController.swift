//
//  ColorPickerViewController.swift
//  wildlights
//
//  Created by Bret Patterson on 6/27/15.
//  Copyright (c) 2015 Bret Patterson. All rights reserved.
//

import Foundation

class ColorPickerViewController : UIViewController {
   @IBOutlet weak var colorPicker  : NKOColorPickerView?
   var onColorChange : ((color: UIColor!) -> (Void))?
    
    
    override func viewDidLoad() {
        colorPicker?.color = UIColor.whiteColor()
        colorPicker?.didChangeColorBlock = onColorChange
    }
}