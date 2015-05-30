//
//  DeviceSettingsViewController.swift
//  wildlights
//
//  Created by Bret Patterson on 5/24/15.
//  Copyright (c) 2015 Bret Patterson. All rights reserved.
//

import UIKit

class DeviceSettingsViewController: UITableViewController, UIGestureRecognizerDelegate {
    @IBOutlet weak var name         : UITextField?
    @IBOutlet weak var brightness   : UISlider?
    @IBOutlet weak var pixelCount   : UITextField?
    @IBOutlet weak var recentColor1 : UIView?
    @IBOutlet weak var recentColor2 : UIView?
    @IBOutlet weak var recentColor3 : UIView?
    @IBOutlet weak var recentColor4 : UIView?
    @IBOutlet weak var recentColor5 : UIView?
    @IBOutlet weak var colorPicker  : NKOColorPickerView?
    var device                      : WildLightsHeadUnit?
    private var lastColorSent       : UIColor?
    
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        self.pixelCount?.text = "\(device!.strip.pixelCount)"
        self.brightness?.value = (Float(device!.strip.brightness) / 255) * 100
        self.name?.text = device?.name
        colorPicker?.color = UIColor.whiteColor()
        
        self.recentColor1?.addGestureRecognizer(UITapGestureRecognizer(target:self, action: "updateWithRecentColor:"))
        self.recentColor2?.addGestureRecognizer(UITapGestureRecognizer(target:self, action: "updateWithRecentColor:"))
        self.recentColor3?.addGestureRecognizer(UITapGestureRecognizer(target:self, action: "updateWithRecentColor:"))
        self.recentColor4?.addGestureRecognizer(UITapGestureRecognizer(target:self, action: "updateWithRecentColor:"))
        self.recentColor5?.addGestureRecognizer(UITapGestureRecognizer(target:self, action: "updateWithRecentColor:"))
        
        colorPicker?.didChangeColorBlock =  { color in
            println ("Selected \(color)")
            if (self.device!.streamReady) {
                self.recentColor5?.backgroundColor = self.recentColor4?.backgroundColor
                self.recentColor4?.backgroundColor = self.recentColor3?.backgroundColor
                self.recentColor3?.backgroundColor = self.recentColor2?.backgroundColor
                self.recentColor2?.backgroundColor = self.recentColor1?.backgroundColor
                self.recentColor1?.backgroundColor = color
                
                self.sendColor(color)
            } else {
                println ("Skipping send since previous send has not completed")
            }
        }
        
        // handle device disconnected error
        NSNotificationCenter.defaultCenter().addObserverForName(WildLightsHeadUnit.WildLightsHeadUnitDisconnected, object:nil, queue: nil) { note in
            dispatch_async(dispatch_get_main_queue(),{
                var userInfo = note.userInfo as! Dictionary<String, WildLightsHeadUnit>
                var headUnit = userInfo[WLDeviceManager.DeviceKey]
                if (headUnit?.identifier == self.device?.identifier) {
                    self.navigationController?.popToRootViewControllerAnimated(true)
                    // display error message
                    dispatch_async(dispatch_get_main_queue(),{
                        var alert = UIAlertView(title: nil, message: "Connection Lost!", delegate: nil, cancelButtonTitle: "Done")
                        alert.show()
                    })
                }
            });
        }
    }
    
    private func sendColor(color: UIColor ) {
        var colors = CGColorGetComponents(color.CGColor)
        var program = LEDMiniProgram(count:self.device!.strip.pixelCount, brightness: UInt8(255),
            statements: [
                PushColorStatement(stripNum:UInt8(0), count: self.device!.strip.pixelCount, red: UInt8(colors[0] * CGFloat(self.device!.strip.brightness)), green: UInt8(colors[1] * CGFloat(self.device!.strip.brightness)), blue: UInt8(colors[2] * CGFloat(self.device!.strip.brightness))),
                UpdateStatement(),
                EndProgramStatement()
            ]
        )
        self.device!.sendMiniProgram(program)
        self.lastColorSent = color
        
    }
    
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
}

extension DeviceSettingsViewController : UITextFieldDelegate {
    
    func textFieldDidEndEditing(textField: UITextField) {
        if (textField == pixelCount) {
            if ((pixelCount!.text.toInt()) != nil) {
                device!.strip.pixelCount = UInt8(pixelCount!.text.toInt()!)
            }
        } else if (textField == name) {
            device!.name = name!.text
        }
    }
}

extension DeviceSettingsViewController  {
    @IBAction func brightnessChanged(sender: UISlider) {
        if (sender == brightness) {
            device!.strip.brightness = UInt8(sender.value/100*255)
            
            if (self.device!.streamReady) {
                sendColor(self.lastColorSent!)
            }
        }
    }
    
    @IBAction func brightnessEditingEnded(sender: UISlider) {
        if (sender == brightness) {
            self.device!.strip.brightness = UInt8(sender.value/100*255)
            self.sendColor(self.recentColor1!.backgroundColor!)

        }
    }
    @IBAction func updateWithRecentColor(sender: AnyObject) {
        var color : UIColor?
        var gesture = sender as! UITapGestureRecognizer
        switch (gesture.view!) {
        case self.recentColor1!:
            color = self.recentColor1?.backgroundColor
        case self.recentColor2!:
            color = self.recentColor2?.backgroundColor
        case self.recentColor3!:
            color = self.recentColor3?.backgroundColor
        case self.recentColor4!:
            color = self.recentColor4?.backgroundColor
        case self.recentColor5!:
            color = self.recentColor5?.backgroundColor
        default:
            break
        }
        if (color != nil && self.device!.streamReady) {
            self.sendColor(color!)
        }
    }
    
    
}
