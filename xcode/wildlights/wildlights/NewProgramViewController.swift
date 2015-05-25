//
//  NewProgramViewController.swift
//  wildlights
//
//  Created by Bret Patterson on 5/23/15.
//  Copyright (c) 2015 Bret Patterson. All rights reserved.
//

import UIKit
import SpriteKit

class NewProgramViewController: UIViewController {
    let cellReuseIdentifier             = "cellReuseIdentifier"
    @IBOutlet weak var cancelButton     : UIBarButtonItem?
    @IBOutlet weak var saveButton       : UIBarButtonItem?
    @IBOutlet weak var toolBar          : UIToolbar?
    @IBOutlet weak var addEditButton    : UIBarButtonItem?
    @IBOutlet weak var deleteButton     : UIBarButtonItem?
    @IBOutlet weak var flexSpace        : UIBarButtonItem?
    @IBOutlet weak var tableView        : UITableView?
    var program                         = LedProgram(pixelCount: 10)
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view.
    }
    
    override func viewWillAppear(animated: Bool) {
    }
    
    @IBAction func cancelAction(sender: AnyObject) {
        self.dismissViewControllerAnimated(true, completion: nil)
    }
    
    
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    override func prepareForSegue(segue: UIStoryboardSegue, sender: AnyObject?) {
        if (segue.identifier == "addStatement") {
            var controller = segue.destinationViewController as! StatementTableViewController
            
            controller.didSelect = { statement in
                 self.program.instructions.append(statement)
                 self.tableView?.reloadData()
            }
        }
    }
    
    /*
    // MARK: - Navigation
    
    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepareForSegue(segue: UIStoryboardSegue, sender: AnyObject?) {
    // Get the new view controller using segue.destinationViewController.
    // Pass the selected object to the new view controller.
    }
    */
    
}

extension NewProgramViewController : UITableViewDataSource {
    
    func numberOfSectionsInTableView(tableView: UITableView) -> Int {
        // #warning Potentially incomplete method implementation.
        // Return the number of sections.
        return 2
    }
    
    func tableView(tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        // #warning Incomplete method implementation.
        // Return the number of rows in the section.
        if (section == 0) {
            return 1
        } else {
            return program.instructions.count
        }
    }
    
    func tableView(tableView: UITableView, cellForRowAtIndexPath indexPath: NSIndexPath) -> UITableViewCell {
        
        if (indexPath.section == 0) {
            var cell = tableView.dequeueReusableCellWithIdentifier(cellReuseIdentifier, forIndexPath:indexPath) as! UITableViewCell
            cell.textLabel?.text = "Settings"
            
            return cell
        } else {
            var cell = tableView.dequeueReusableCellWithIdentifier(cellReuseIdentifier, forIndexPath:indexPath) as! UITableViewCell
            cell.textLabel?.text = program.instructions[indexPath.row].name
            return cell
        }
        
        
    }
}