arduino-buildbot-client
=======================

Watch a buildbot master, turn on a green LED if everything is fine, a red one if any build on the master has failed.

Setup
-----

The Arduino (Uno at least) has not enough memory to directly use the Buildbot JSON API.
This is why a PHP parser is present in the server directory. The only thing it does is
1. Parsing the JSON output
2. Write 1 if there's any error, 0 otherwise

### Server

Simply drop the PHP parser in your webserver directory

### Arduino

The electronic part is realy simple: only plug a red LED between the pin 4 and ground (using a resistor…),
and a green LED between the pin 5 and ground (with, again, a resistor).

You need to edit the main.ino, replace the first lines:
 * Arduino's MAC and IP addresses
 * Your webserver's IP
 * At line 78, the path to the PHP wrapper (currently /bb-arduino.php)
 * Optionaly, the LEDs pins (failPin and okPin)
 * Optionaly, the check frequency (postingInterval), in milliseconds (5 minutes default)
