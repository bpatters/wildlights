//
//  StatementTableViewController.swift
//  wildlights
//
//  Created by Bret Patterson on 5/24/15.
//  Copyright (c) 2015 Bret Patterson. All rights reserved.
//

import UIKit
typealias didSelectResponder = (statement: Statement) -> Void

class StatementTableViewController: UITableViewController {
    let statementCell = "statementCell"
    var statements : [Statement] = []
    var didSelect : didSelectResponder?
    
    override func viewDidLoad() {
        super.viewDidLoad()

        // Uncomment the following line to preserve selection between presentations
        // self.clearsSelectionOnViewWillAppear = false

        // Uncomment the following line to display an Edit button in the navigation bar for this view controller.
        // self.navigationItem.rightBarButtonItem = self.editButtonItem()
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }

}

extension StatementTableViewController : UITableViewDataSource {
    
    
    override func numberOfSectionsInTableView(tableView: UITableView) -> Int {
        // #warning Potentially incomplete method implementation.
        // Return the number of sections.
        return 1
    }

    override func tableView(tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        // #warning Incomplete method implementation.
        // Return the number of rows in the section.
        return statements.count
    }
    override func tableView(tableView: UITableView, cellForRowAtIndexPath indexPath: NSIndexPath) -> UITableViewCell {
        let cell = tableView.dequeueReusableCellWithIdentifier("statementCell", forIndexPath: indexPath) as! UITableViewCell
        
        cell.textLabel?.text = statements[indexPath.row].name
        
        return cell
    }
}


extension StatementTableViewController : UITableViewDelegate {
    
    override func tableView(tableView: UITableView, didSelectRowAtIndexPath indexPath: NSIndexPath) {
        didSelect?(statement:statements[indexPath.row])
        self.dismissViewControllerAnimated(false, completion: nil)
    }
}
