//
//  PushFadeStatement.swift
//  wildlights
//
//  Created by Bret Patterson on 5/28/15.
//  Copyright (c) 2015 Bret Patterson. All rights reserved.
//

import Foundation


class PushLerpStatement : Statement {
    var name       : String = "Push Fade"
    var stripNum   : UInt8
    var count      : UInt8
    var startRed   : UInt8
    var startGreen : UInt8
    var startBlue  : UInt8
    var endRed     : UInt8
    var endGreen   : UInt8
    var endBlue    : UInt8
    
    required init (var stripNum : UInt8, var count : UInt8, var startRed : UInt8, var startGreen : UInt8, var startBlue : UInt8,
        var endRed : UInt8, var endGreen : UInt8, var endBlue : UInt8) {
            
            self.stripNum = stripNum
            self.count = count
            self.startRed = startRed
            self.startGreen = startGreen
            self.startBlue = startBlue
            self.endRed = endRed
            self.endGreen = endGreen
            self.endBlue = endBlue
    }
    
    func toByteCode() -> [UInt8] {
        return [UInt8(Instructions.INST_PUSH_LERP_I), UInt8(count), startRed, startGreen, startBlue, endRed, endGreen, endBlue]
    }
    
}