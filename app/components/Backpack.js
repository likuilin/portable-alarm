import React from 'react';
import { StyleSheet, Text, View, Button, Image } from 'react-native';
import { Alert } from 'react-native';

export default class Backpack extends React.Component {
    state = { visible: true }
    
    send = e => { if (this.props.hookup) fetch(e) }
    
    render() {
        if (!this.state.visible) return null;
        return (
            <View style={styles.backpack}>
                <Text>asdf</Text>
                <Image source={require("../assets/icon.png")} style={styles.img} />
                <Text style={styles.title}>{this.props.name}</Text>
                <View style={styles.buttons}>
                    <Button style={styles.button} title="Mute Alarm" onPress={()=>this.send("https://kuilin.net/portalert/write.php?data=nope")} />
                    <Button style={styles.button} title="Activate Alarm" onPress={()=>this.send("https://kuilin.net/portalert/write.php?data=ALARM_ON")} />
                    <Button style={styles.button} color="red" title="Delete Thing" onPress={()=>this.setState({visible: false})} />
                </View>
                <Text style={styles.status}>{this.props.last ? "Last Notification: " + this.props.last : ""}</Text>
            </View>
        );
    }
}

const styles = StyleSheet.create({
    backpack: {
        backgroundColor: '#f5be5f',
        width: '90%',
        height: 70,
        marginLeft: 50,
        marginRight: 50,
        marginTop: 20,
        borderRadius: 20
    },
    img: {
        width: 70,
        height: 70,
        position: 'absolute'
    },
    buttons: {
        flexDirection: "row",
        right: 0,
        top: 4,
        position: "absolute"
    },
    button: {
        alignItems: "center",
    },
    title: {
      fontSize: 30,
      paddingLeft: 20,
      left: 70,
    },
    status: {
        fontSize: 14,
        position: "absolute",
        right: 10,
        bottom: 4
    }
});
