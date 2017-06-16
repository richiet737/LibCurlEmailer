# LibCurlEmailer
Example of using LibCurl to send emails with attachments written in C++

LibCurl Emailer Having spent weeks trying to work out how to send an Email with a pdf attachment using C++ 
I thought I would share what I have come up with. This is my first time using curl so if there are any errors 
or better ways of doing things then please let me know. I have commented the class files so it should be fairly 
easy to understand what is going on. I have, of course used code snippets that I have found online. I am sorry 
but I have visited so many sites trying to get this to work I am unsure of where I got various bits. 
If you recognise your work then let me know and I will add you to the credits.

The project consists of 2 classes SMTPCompiler and Base64Codec. The first provides all the functionality for 
creating and sending an email. The second is a utility for converting a binary file into a base 64 string. 
File attachments need to be chunked into 76 byte chunks for sending this is handled by SMTPCompiler.

CRLF = "\r\n 
BOUNDARY = a random string for delimiting parts of the email e.g. header, body, attachments 
SMTP_TERMINATOR = '.'

Please note: If you edit this code make sure that you retain exactly the positioning of the CRLFs and the 
BOUNDARIES otherwise you will be in a world of pain. Also there is a very sneaky terminator of the SMTP message 
that is a single dot. To help highlight this I have declared a const char called SMTP_TERMINATOR. 
Don't miss it off. Enjoy Rich
