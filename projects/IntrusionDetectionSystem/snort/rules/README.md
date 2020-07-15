# Rule system of snort

The rules are what snort is using to check the traffic. They are written in a text language which snort is reading and processing.
The rule syntax we use is mostly used with snort 2 but works without problems in snort 3. You can also
convert the snort.conf file to a snort.lua file, which is used in snort3, with
````
snort2lua -c snort.conf
````

## Rule Writing

Before you begin to write a rule, you have to tell snort where the rule file is. For that open
the snort.conf file with the text editor of your choice. Scroll down till you see (under step 7):

````
include $RULE_PATH/LICSTER_SSH.rules
include $RULE_PATH/LICSTER_Modbus.rules
include $RULE_PATH/LICSTER_ICMP.rules
include $RULE_PATH/LICSTER_HTTP.rules
````

These rules are custom rules which work with the LICESTER-Testbed.
Now you can choose to use the .rules files which are already there or create a new rule file.
To create a new rule file type in
````
include $RULE_PATH/Name-of-file.rules
````
Now you have to go under path/to/ids-for-licster/snort/rules and create a Name-of-file.rules file.
Now you can create your first rules.

### Rule Syntax

The syntax which snort uses is not as difficult as it at first seems.
````
alert tcp $EXTERNAL_NET any -> $HOME_NET any (msg:"What the rule does", ... , sid=1000000, ref=1)
```` 
alert: The first part of the rule is the action which snort should do if are rule is triggered. In our case snort generates an alert and logs the packet.

tcp: Now we say snort which protocol it should use

$EXTERNAL_NET any: now we are telling snort which ip adresse it should care about if a package is sent their and from which port the package was sent.

$HOME_NET any: Now it is the other way around. Which ip address should it care about if it is receiving a package and also the port were it receives the package.

The next part of the rule is where we specify the content or type of the package

The first part is always an “msg: […] “where we give a short description about what the rule should do.

The next part of the rule is about the option which it should use. Snort uses many different options
so the best way is to go to their [documentaition](http://manual-snort-org.s3-website-us-east-1.amazonaws.com/node27.html)
and search for the option you need.

The last part are the sid numbers. Every rule has one specific number. The first 1.000.000 numbers are reserved by snort.
For our rules we used the range 1001000-1004000.

The rev number is there to tell which version of the rule it is.

###LICSTER rules

The 4 files LICSTER_HTTP.rules, LICSTER_ICMP.rules, LICSTER_Modbus.rules und LICSTER_SSH.rules are the rule files which include
the specific rules for LICSTER.

LICSTER_SSH.rules: reserved sid range 1000000-1000999, Specific rules for ssh

LICSTER_Modbus.rules: reserved sid range 1001000-1001999, Specific rules for Modbus

LICSTER_HTTP.rules: reserved sid range 1002000-1002999, Specific rules for HTTP

LICSTER_ICMP.rules: reserved sid range 1003000-1003999, Specific rules for ICMP
 

