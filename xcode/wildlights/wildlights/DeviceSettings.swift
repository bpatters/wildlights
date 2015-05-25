//
//  DeviceSettings.swift
//  wildlights
//
//  Created by Bret Patterson on 5/25/15.
//  Copyright (c) 2015 Bret Patterson. All rights reserved.
//

import Foundation

class DeviceSettings {
    var name          : String
    var identifier    : String
    var recentColors  : [UInt32] = []
    var program       : LedProgram
    var onDiscMiniProgram : LEDMiniProgram
    
    init (name: String, identifier: String, program: LedProgram = LedProgram(), onDiscMiniProgram: LEDMiniProgram = .DoNothing) {
        self.name = name
        self.identifier = identifier
        self.program = program
        self.onDiscMiniProgram = onDiscMiniProgram
    }
}