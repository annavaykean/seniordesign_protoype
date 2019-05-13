import 'package:flutter/material.dart';
import 'package:flutter_blue/flutter_blue.dart';
import 'dart:async';

class BluetoothTestScreen extends StatefulWidget {
  @override
  BluetoothState createState() => BluetoothState();
}
class BluetoothState extends State<BluetoothTestScreen> {

  FlutterBlue flutterBlue = FlutterBlue.instance;
  StreamSubscription scanner;
  Map<DeviceIdentifier, ScanResult> scanResults = new Map();
  List<BluetoothDevice> deviceList = [];
  BluetoothDevice device;
  bool connected = false;
  bool pressed = false; //am I using this?

  @override
  Widget build(BuildContext context) {
    return new Scaffold(
        appBar: AppBar(
          title: const Text('This is for testing Bluetooth!'),
        ),
        body: Column(
            children: <Widget> [
              RaisedButton(
                child: const Text('Search for devices'),
                onPressed: () => scanForDevices(),
              ),
              Column (
                children: <Widget>[
                  listDevices()
                ]
              )
            ]
        )
    );
  }

  scanForDevices() {
    print('INFO: Scanning for bluetooth devcies...');
    scanner = flutterBlue.scan(timeout: const Duration(seconds: 5)).listen((scanResult){
      print('INFO: deviceID = ${scanResult.device.id}');
      deviceList.add(scanResult.device);
      setState((){
        scanResults[scanResult.device.id] = scanResult;
      });
    }, onDone: stopScanning());

  }
  stopScanning() {
    if(scanner == null){
      print("INFO: Attempted to cancel the scanner, but it was null");
    } else {
      scanner?.cancel();
      scanner = null;
    }
    listDevices();
  }

  Widget listDevices() {
    List<Widget> list = new List<Widget>();
    print('INFO: scanResults length = ' + scanResults.length.toString());
    for(var i=0;i<scanResults.length; i++){
      list.add(new Text(scanResults[i].toString()));
      if(scanResults[i] != null) {
        print('INFO: SR = ' + scanResults[i].device.id.toString());
      }
    }
    if(list == null){
      print('INFO: the list for display is null :(');
    }
    return new Row(children: list);
  }
}