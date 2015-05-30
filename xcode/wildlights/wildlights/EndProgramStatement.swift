//
//  EndProgramStatement.swift
//  wildlights
//
//  Created by Bret Patterson on 5/25/15.
//  Copyright (c) 2015 Bret Patterson. All rights reserved.
//

import Foundation

class EndProgramStatement : Statement {
    var name      : String             = "End Program"
    
    required init() {
        
    }
    
    func toByteCode() -> [UInt8] {
        return [UInt8(Instructions.INST_END)]
    }
}