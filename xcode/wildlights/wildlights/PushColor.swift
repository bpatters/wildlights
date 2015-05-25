//
//  PushColor.swift
//  wildlights
//
//  Created by Bret Patterson on 5/24/15.
//  Copyright (c) 2015 Bret Patterson. All rights reserved.
//

import Foundation
import UIKit

class PushColor : Statement {
    var name      : String  = "Push Color"
    var stripNum  : UInt8   = 0
    var count     : UInt8   = 0
    var red       : UInt8   = 0
    var green     : UInt8   = 0
    var blue      : UInt8   = 0
    
    required init() {
        
    }
    
    init (stripNum: UInt8, count: UInt8, red: UInt8, green: UInt8, blue: UInt8) {
        self.stripNum = stripNum
        self.count = count
        self.red   = red
        self.green = green
        self.blue  = blue
    }
    
    required init(byteCode: [UInt8]) {
        count = byteCode[0]
        green = byteCode[1]
        red   = byteCode[2]
        blue  = byteCode[3]
    }
    
    func toByteCode() -> [UInt8] {
       return [UInt8(Instructions.INST_PUSHCR), stripNum, count, red, green, blue]
    }
    
}