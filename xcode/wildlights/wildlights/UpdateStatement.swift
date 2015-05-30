//
//  SyncStatement.swift
//  wildlights
//
//  Created by Bret Patterson on 5/24/15.
//  Copyright (c) 2015 Bret Patterson. All rights reserved.
//

import Foundation

class UpdateStatement : Statement {
    var name      : String             = "Update"
    var stripNum  : UInt8              = 0;
    
    required init() {
        
    }
    
    func toByteCode() -> [UInt8] {
        return [UInt8(Instructions.INST_UPDATE), stripNum]
    }
}