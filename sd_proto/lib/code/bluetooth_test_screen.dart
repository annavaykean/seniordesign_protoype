import 'package:flutter/material.dart';
import 'package:flutter_blue/flutter_blue.dart';
import 'dart:async';
import 'package:sd_proto/main.dart';

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
    return ListTile(
      title: Text(
      //  data.advertisementData.localName.toString() == ''? data.id.toString(): data.advertisementData.localName.toString(),
      //  data.id.toString(),
        data.name.toString() == ''? data.id.toString(): data.name.toString(),
      ),
      trailing: Icon(
        Icons.favorite,
      ),
      onTap: () {
        setState(() {
          if(connected) {
           // disconnectFromDevice();
            print("INFO: Device is already connected!");
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
                child: const Text('Show test page'),
                onPressed: () => Navigator.of(context).pushNamed('/SendBluetoothData'),
              ),
              RaisedButton(
                child: const Text('Search for devices'),
                onPressed: () => scanForDevices(),
              ),
              Expanded(
                child: Container(
                child: buildDeviceList(),
              )
              )
            ]
        )
    );
  }

  scanForDevices() {
    deviceList = [];
    print('INFO: Scanning for bluetooth devices...');
    scanner = flutterBlue.scan(timeout: const Duration(milliseconds: 300)).listen((scanResult) async{

      scanResult.advertisementData.localName.toString() == ''? print('INFO: deviceID = ${scanResult.device.id}'): print('INFO: deviceName = ${scanResult.advertisementData.localName} (${scanResult.device.id})');
      //check to see if device has already been added to deviceList
      bool isDuplicate = false;
      for(int i=0;i<deviceList.length;i++){
        if(deviceList[i].id == scanResult.device.id) {
          isDuplicate = true;
          break;
        }
      }
      setState((){
        //to prevent lots of results, change the if condition
        //to see if the device ID matches the id associated with each
        //piece of hardware
        if(deviceList == null || !isDuplicate) {
          deviceList = List.from(deviceList);
          deviceList.add(scanResult.device);
          print('deviceList');
          for(int i=0;i<deviceList.length;i++){
            print(deviceList[i].id);
          }
          print('INFO: adding ${scanResult.device} to list of available devices');
        } else {
          print('INFO: ${scanResult.device} is already in the list of available devices, skipping duplicate');
        }
      });
    }, onDone: stopScanning());


  }
  stopScanning() {
    if(scanner == null){
      print("INFO: Attempted to cancel the scanner, but it was null");
    } else {
      print('INFO: Scanner has been terminated');
      scanner?.cancel();
      scanner = null;
    }
    buildDeviceList();
  }

  disconnectFromDevice() {
    connected = false;
    return null;
  }

  connectToDevice(BluetoothDevice device) {
    deviceConnection = flutterBlue.connect(device, timeout: const Duration(seconds: 20), autoConnect: false).listen(null);
    deviceStateSubscription = device.onStateChanged().listen((s) {
      if(s == BluetoothDeviceState.connected) {
        connected = true;
      print("INFO: Connection Successful!");
      stopScanning();
      MyApp.device = device;
      MyApp.deviceConnection = deviceConnection;
      Navigator.of(context).pushNamed('/SendBluetoothData');
      } else {
        print("INFO: Failed to connect to " + device.id.toString());
      }
    });

  }

}
