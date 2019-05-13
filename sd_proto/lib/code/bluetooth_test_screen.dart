import 'package:flutter/material.dart';
import 'package:flutter_blue/flutter_blue.dart';
import 'dart:async';

class BluetoothTestScreen extends StatefulWidget {
  @override
  BluetoothState createState() => BluetoothState();
}
class BluetoothState extends State<BluetoothTestScreen> {
  List<Widget> list = new List<Widget>();
  FlutterBlue flutterBlue = FlutterBlue.instance;
  StreamSubscription scanner;
  //Map<DeviceIdentifier, ScanResult> scanResults = new Map();

  List<BluetoothDevice> deviceList = [];
  BluetoothDevice device;
  bool connected = false;
  bool pressed = false; //am I using this?

  Widget buildRow(var data){
    return ListTile(
      title: Text(
        data.id.toString(),
      )
    );
  }
  Widget buildDeviceList(){
    return ListView.builder(
      padding: const EdgeInsets.all(16.0),
      itemBuilder: (context, i) {
        if(deviceList.length > i) {
          return buildRow(deviceList[i]);
        }
      },
      shrinkWrap: true,
    );
  }

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
              Container(
                child: buildDeviceList(),
              )
            ]
        )
    );
  }

  scanForDevices() {
    print('INFO: Scanning for bluetooth devcies...');
    scanner = flutterBlue.scan(timeout: const Duration(milliseconds: 500)).listen((scanResult) async{
      print('INFO: deviceID = ${scanResult.device.id}');
      setState((){
        //to prevent lots of results, change the if condition
        //to see if the device ID matches the id associated with each
        //piece of hardware
        if(!deviceList.contains(scanResult.device)) {
          deviceList.add(scanResult.device);
        }
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
    buildDeviceList();
  }

/*  Widget listDevices() {
    print('INFO: scanResults length = ' + deviceList.length.toString());
    for(var i=0;i<deviceList.length; i++){
      for(var j=0;j<deviceList.length; j++) {
        if(deviceList.contains())
      }
      list.add(new Text(deviceList[i].id.toString()));
      if(deviceList[i] != null) {
        print('INFO: SR = ' + deviceList[i].id.toString());
      }
    }
    if(list == null){
      print('INFO: the list for display is null :(');
    }
    return new Row(children: list);
  }*/
}