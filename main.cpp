//
//  main.cpp
//  Email_test_project
//
//  Created by Richard Bradshaw on 30/05/2017.
//  Copyright © 2017 Richard Bradshaw. All rights reserved.
//  Ignore the copyright do what you like with this code
//

#include <iostream>
#include "SMTPCompiler.hpp"

using namespace std;

/**
 * Example code for creating and sending an email with 2 pdf attachments
 * You can send as many attachments as you like just add the file names to the vector<string>
 */

int main(int argc, const char * argv[]) {
    
    SMTPCompiler s;
    string from = "myemail@gmail.com";
    s.setFrom(from);
    string to = "your@gmail.com";
    string cc = "someone_else@gmail.com";
    vector<string> recipients;
    recipients.push_back(to);
    recipients.push_back(cc);
    s.setDestinations(recipients);
    string server = "smtp.gmail.com:25";
    s.setServer(server);
    string username = "myemail@gmail.com";
    s.setUsername(username);
    string pwd = "MyPassword";
    s.setPassword(pwd);
    string subject = "The subject text goes here";
    s.setSubject(subject);
    
    
    string body = "this is the body text blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah";
    s.setBodyText(body);
    
    string bodyContent = "application/pdf";
    s.setBodyContentType(bodyContent);
    string bodyEncoding = "base64";
    s.setBodyEncoding(bodyEncoding);
    
    vector<string> attachments;
    string attachment = "myPDFFile.pdf";
    string attachment2 = "myOtherPDFFile.pdf";
    attachments.push_back(attachment);
    attachments.push_back(attachment2);
    s.setAttachmentList(attachments);
 
    s.send();
    //s.print();
    
    return 0;
}
