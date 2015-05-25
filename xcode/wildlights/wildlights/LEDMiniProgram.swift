//
//  LEDMiniProgram.swift
//  wildlights
//
//  Created by Bret Patterson on 5/25/15.
//  Copyright (c) 2015 Bret Patterson. All rights reserved.
//

import Foundation


class LEDMiniProgram {
    static var DoNothing = LEDMiniProgram(count: 0, brightness: 0, statements: [EndProgramStatement()])
    var count      : UInt8 = 10;
    var brightness : UInt8 = 64;
    var statements : [Statement] = []
    
    init (count: UInt8, brightness: UInt8, statements: [Statement]) {
        self.count = count;
        self.brightness = brightness;
        self.statements = statements;
    }
    
    func  getDataPacket() -> NSData {
        assert(statements[statements.count-1] is EndProgramStatement,  "Mini programs must end with EndProgramStatement")
        assert(statements.count <= 10, "Mini programs must be less than 10 instructions")
        
        var data = NSMutableData()
        var bytes : [UInt8]
        
        data.appendData(NSData(bytes:[UInt8(count), UInt8(brightness)], length:2))
        for statement in statements {
            bytes = statement.toByteCode()
            data.appendData(NSData(bytes:bytes, length:bytes.count))
        }
        
        assert(data.length < 20, "Mini programs must be less than 20 bytes total")
        
        return data
    }

}