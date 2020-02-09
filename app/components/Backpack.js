import React from 'react';
import { StyleSheet, Text, View, Button, Image } from 'react-native';
import { Alert } from 'react-native';

export default class Backpack extends React.Component {
    state = { visible: true, armed: false }
    
    mute = () => {
        // if (!this.props.hookup) return;
        this.send("https://kuilin.net/portalert/write.php?data=nope");
    }
    
    alarm = () => {
        // if (!this.props.hookup) return;
        if (!this.state.armed) Alert.alert("Arm it first.");
        this.send("https://kuilin.net/portalert/write.php?data=ALARM_ON");
    }
    
    send = e => { if (this.props.hookup) fetch(e) }
    
    togarm = () => {
        this.setState(state => {
            fetch("https://kuilin.net/portalert/arm.php?data=" + ((!state.armed) ? "armed" : "disarmed"));
            return {armed: !state.armed};
        });
    }
    
    render() {
        if (!this.state.visible) return null;
        return (
            <View style={styles.backpack}>
                <Text>asdf</Text>
                <Image source={require("../assets/icon.png")} style={styles.img} />
                <Text style={styles.title}>{this.props.name}</Text>
                <View style={styles.buttons}>
                    <Button style={styles.button} title={this.state.armed ? "Disarm" : "Arm"} onPress={this.togarm} />
                    <Button style={styles.button} title="Mute Alarm" onPress={this.mute} />
                    <Button style={styles.button} title="Activate Alarm" onPress={this.alarm} />
                    <Button style={styles.button} color="red" title="Delete" onPress={()=>this.setState({visible: false})} />
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
