const SerialPort = require('serialport');
const Readline = require('@serialport/parser-readline');

const { Expo } = require('expo-server-sdk');
let expo = new Expo();

const port = new SerialPort("COM4", { baudRate: 9600 });
const parser = new Readline();
port.pipe(parser);

const pushToken = "ExponentPushToken[qM8iaqBEAK5QA0Mehrtl1G]";

parser.on('data', line => {
    // Check that all your push tokens appear to be valid Expo push tokens
    if (!Expo.isExpoPushToken(pushToken)) {
        console.error(`Push token ${pushToken} is not a valid Expo push token`);
        return;
    }

    expo.sendPushNotificationsAsync([{
        to: pushToken,
        sound: 'default',
        body: line,
        data: { line: line.trim(), date: +new Date() },
    }]);
    console.log(`> ${line}`);
});
