import React from 'react';
import { StyleSheet, Text, View } from 'react-native';
import { AppState, Button, Alert } from 'react-native';

import { Notifications } from 'expo';
import * as Permissions from 'expo-permissions';
import Constants from 'expo-constants';

import Backpack from './components/Backpack';

export default class App extends React.Component {
    state = {
        appState: AppState.currentState,
        loaded: false,
        token: "",
        notification: "",
        childKey: 123
    }
    
    componentDidMount() {
        AppState.addEventListener('change', this._handleAppStateChange);
        this._load();
        Notifications.addListener(this._handleNotification);
    }
    
    _handleNotification = notification => {
        console.log("123");
        console.log(notification);
        this.setState({ notification: notification });
    };
    
    componentWillUnmount() {
        AppState.removeEventListener('change', this._handleAppStateChange);
    }
    
    _handleAppStateChange = nextAppState => {
        if (this.state.appState.match(/inactive|background/) && nextAppState === 'active') {
            this._load();
        }
        this.setState({ appState: nextAppState });
    };
    
    async _load() {
        // Register an event handler and 
        // Clear all notifications. Throws on Android.
        try {
            await Notifications.setBadgeNumberAsync(0);
        } catch (_) {};
        
        // Request notifications
        const { status } = await Permissions.askAsync(Permissions.NOTIFICATIONS);
        if (status !== 'granted') return this.setState({loaded: true, token: ""});
        
        // Get token
        let token = await Notifications.getExpoPushTokenAsync();
        this.setState({loaded: true, token });
    }
    
    render() {
        console.log(this.state.token);
        if (!this.state.loaded) return null;
        else return (
            <View style={styles.container}>
                <View style={styles.topbar}>
                    <Text style={styles.text}>My Things</Text>
                    <View style={styles.logout}>
                        <Button color="red" title="Log Out" onPress={()=>this.setState({childKey: Math.random()})} />
                    </View>
                </View>
                <Backpack key={this.state.childKey + 1} name="My Backpack" last="test (123)" />
                <Backpack key={this.state.childKey + 2} name="Second Backpack" />
                <Backpack key={this.state.childKey + 3} name="Third Backpack" last={"Battery Low at " + new Date().toLocaleString()} />
                <Backpack key={this.state.childKey + 4} name="Why Do I Have Four Backpacks" />
                <Backpack key={this.state.childKey + 5} name="Baaacccckkkkppppaaaaccckkkksssss" last={"Movement Detected at " + new Date().toLocaleString()} />
                <Backpack key={this.state.childKey + 6} name="❤️" />
            </View>
        );
    }
}

const styles = StyleSheet.create({
    container: {
        flex: 1,
        flexDirection: "column",
        padding: 30,
        backgroundColor: '#eee',
    },
    topbar: {
        width: '100%',
        height: 60,
        borderBottomWidth: 1
    },
    text: {
        fontSize: 50,
        width: '90%',
        textAlign: 'left',
    },
    logout: {
        position: 'absolute',
        right: 0,
        bottom: 0,
        alignItems: 'flex-end'
    }
});
