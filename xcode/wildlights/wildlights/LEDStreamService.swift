//
//  LEDStreamService.swift
//  wildlights
//
//  Created by Bret Patterson on 5/17/15.
//  Copyright (c) 2015 Bret Patterson. All rights reserved.
//

import Foundation
import CoreBluetooth



class LEDStreamService {
    private var device : CBPeripheral
    var service : CBService
    var streamChar : CBCharacteristic?
    var syncChar : CBCharacteristic? {
        didSet {
            if (syncChar != nil) {
                NSNotificationCenter.defaultCenter().removeObserver(LEDStreamCharUUIDString)
            }
            NSNotificationCenter.defaultCenter().addObserverForName(LEDStreamCharUUIDString, object:nil, queue: nil) { note in
                let userData = (note.userInfo as NSDictionary!)
                let value = self.syncChar!.value;
                self.ready = UInt8(value.bytes[0]) == 0;
            }
        }
    }
    var ledStrip : LedStrip
    var ready : Bool
    
    init(peripheral _device: CBPeripheral, service _service: CBService) {
        
        self.device = _device
        self.service = _service
        self.streamChar = nil
        self.syncChar = nil
        self.ledStrip = LedStrip()
        self.ready = false
    }
    
    func sync() {
        var data = NSMutableData()
        
        writeData(NSData(bytes: [UInt8(0), UInt8(1), UInt8(1)], length: 3))

        println ("Wrote \(data) to LED Strip Service: \(service.UUID) Char:\(streamChar?.UUID)")
     
        //println ("Sample data \(sampleData)")
    }
    
    func update() {
        assert(ready, "You must wait sync to finish before sending more updates")
        sendPixels()
        sendHeader(false)
    }
    
    func updateWithSync() {
          assert(ready, "You must wait sync to finish before sending more updates")
        sendPixels()
        sendHeader(true)
    }
    
    func waitForReady() {
        while (!ready) {
            sleep(10)
        }
    }
    private func sendHeader(sync: Bool) {
        // 00 00 offset + Sync Byte + Brightness Byte + Pixel Count Byte
        writeData(NSData(bytes: [UInt8(0), UInt8(0), UInt8(sync ? 1 : 0), ledStrip.brightness, ledStrip.pixelCount], length:5))
        ready = !sync;
    }


    private func sendPixels() {
        var pixelOffset = 3
        var offset = 0
        var chunkSize = 16 - 2 // 20 byte limit -2 for offset value
        var pixelDataCount = Int(ledStrip.pixelCount*3)
        var pixelData = NSData(bytes: ledStrip.pixels as [UInt8], length:Int(ledStrip.pixelCount)*3)
        
        while (offset < pixelDataCount) {
            var data = NSMutableData()
            // 2 byte offset header
            data.appendBytes([UInt8((offset & 0xff00) >> 8), UInt8((offset & 0xff) + pixelOffset)], length: 2);

            // append pixel data
            if (offset + chunkSize >= pixelDataCount) {
                data.appendData(pixelData.subdataWithRange(NSRange(location: offset, length: pixelDataCount - offset)))
            } else {
                data.appendData(pixelData.subdataWithRange(NSRange(location: offset, length: chunkSize)))
            }
            writeData(data)
            offset += chunkSize
        }
    }
    
    private func writeData(data: NSData) {
        device.writeValue(data, forCharacteristic: streamChar!, type: CBCharacteristicWriteType.WithResponse)
        println ("sending packet \(data) to device")
    }
}