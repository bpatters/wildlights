//
//  Statement.swift
//  wildlights
//
//  Created by Bret Patterson on 5/24/15.
//  Copyright (c) 2015 Bret Patterson. All rights reserved.
//

import Foundation

protocol Statement {
    var name        : String { get }
    
    func toByteCode() -> [UInt8]
}