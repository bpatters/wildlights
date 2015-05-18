//
//  LedStrip.swift
//  wildlights
//
//  Created by Bret Patterson on 5/16/15.
//  Copyright (c) 2015 Bret Patterson. All rights reserved.
//

import Foundation

class LedStrip {
    var sync : UInt8 = 0  // whether to update the strip on the next opportunity
    var brightness : UInt8 = 64 // scaled brightness of the pixels 0-255
    // number of pixels in the strip
    var pixelCount : UInt8  {
        didSet {
            pixels = Array<UInt8>(count: Int(pixelCount*3), repeatedValue: 0)
        }
    }
    var pixels : [UInt8]  // GRB array of bytes
    
    init() {
        self.sync = 0
        self.brightness = 64
        self.pixelCount = 10
        self.pixels = [UInt8](count: Int(pixelCount*3), repeatedValue: 0)
    }
    
    func setPixel(num: Int, red: Int, green: Int, blue: Int) {
        let index = Int(num)*3
        pixels[index]   = UInt8((green * Int(brightness)) >> 8)
        pixels[index+1] = UInt8((red * Int(brightness)) >> 8)
        pixels[index+2] = UInt8((blue * Int(brightness)) >> 8)
    }
}