import 'package:flutter/material.dart';
import 'package:flutter_blue/flutter_blue.dart';
import 'dart:async';

class BluetoothTestScreen extends StatefulWidget {
  @override
  BluetoothState createState() => BluetoothState();
}
class BluetoothState extends State<BluetoothTestScreen> {

  FlutterBlue flutterBlue = FlutterBlue.instance;

  //For Scanning
  StreamSubscription scanner;
  List<BluetoothDevice> deviceList = [];

  //For Device connect / disconnect
  BluetoothDevice device;
  bool connected = false;
  StreamSubscription deviceConnection;
  StreamSubscription deviceStateSubscription;
  List<BluetoothService> services = new List();
  Map<Guid, StreamSubscription> valueChangedSubs = {};
  BluetoothDeviceState deviceState = BluetoothDeviceState.disconnected;

  Widget buildRow(var data){
    final alreadyConnected = connected;
    return ListTile(
      title: Text(
        data.id.toString(),
      ),
      trailing: Icon(
        Icons.favorite,
      ),
      onTap: () {
        setState(() {
          if(alreadyConnected) {
            disconnectFromDevice();
          }
          else {
            connectToDevice(data);
          }
        });
      }
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

  disconnectFromDevice() {
    return null;
  }

  connectToDevice(BluetoothDevice device) {
    deviceConnection = flutterBlue.connect(device, timeout: const Duration(seconds: 20), autoConnect: false).listen(null);
    deviceStateSubscription = device.onStateChanged().listen((s) {
      if(s == BluetoothDeviceState.connected) {
      print("INFO: Connection Successful!");
      stopScanning();
      device.discoverServices().then((s) {
        print('INFO: Connected to ' + device.id.toString());
        services = s;
        services.forEach((service) {
          print('Services: ' + service.toString());
        });
      });
      } else {
        print("INFO: Failed to connect to " + device.id.toString());
      }
    });

  }
}