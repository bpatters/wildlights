//
//  LedProgram.swift
//  wildlights
//
//  Created by Bret Patterson on 5/16/15.
//  Copyright (c) 2015 Bret Patterson. All rights reserved.
//

import Foundation

let STATE_IDLE               : UInt8	= 0 // do nothing
let STATE_RUNNING            : UInt8    = 1 // running
let STATE_PROGRAMMING        : UInt8    = 2 // being programmed

// defines triggers for state transitions
let MODE_OFF                 : UInt8 =  0 // Turn off all led's and then go idle
let MODE_IDLE                : UInt8 =  1 // Go and stay idle at the next opportunity
let MODE_RUN                 : UInt8 =  2 // Run at the next opportunity if not already running
let MODE_RUN_WHEN_DISCONNECT : UInt8 =  3 // waiting to enter disconnected state before running

class LedProgram {
    var name        : String = "Program"
    var mode        : UInt8           // Mode the program operates in
    var state       : UInt8        	// 1 uint8_t state
    var pc          : UInt16          	// 2 uint8_t pc
    var program     : [UInt8]  // 2k of program space
    var sp          : UInt32;
    var stack       : [UInt32]
    var vars        : [UInt32]    	// 1k uint8_t of 32 bit variables
    var flags       : UInt32    		// 4 uint8_t of flags
    var strips      : [LedStrip] = []  	// 4+ num leds*3
    // interpretation of the opcodes
    var instructions : [Statement] = []
    
    init(pixelCount: UInt8 = 15) {
        self.mode       = MODE_OFF           // Mode the program operates in
        self.state      = STATE_IDLE         // 1 uint8_t state
        self.pc         = 0          	// 2 uint8_t pc
        self.program    = Array<UInt8>(count:1024, repeatedValue: 0)  // 2k of program space
        self.sp         = 0;
        self.stack      = Array<UInt32>(count:16, repeatedValue:0)
        self.vars       = Array<UInt32>(count:32, repeatedValue:0)    	// 1k uint8_t of 32 bit variables
        self.flags      = 0    		// 4 uint8_t of flags
        self.strips.append(LedStrip(count: pixelCount))
    }
}