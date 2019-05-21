import 'package:flutter/material.dart';
import 'package:flutter_blue/flutter_blue.dart';
import 'package:sd_proto/main.dart';
//send and receive numbers (for testing purposes only)
class SendBluetoothData extends StatefulWidget {
  @override
  SendBluetoothDataState createState() => SendBluetoothDataState();
}

class SendBluetoothDataState extends State<SendBluetoothData> {
  TextEditingController numToSendCtrl = new TextEditingController();
  int numReceived = 0;
  String numToSend = '0';
  @override
  Widget build(BuildContext context) {
    return new Scaffold(
        appBar: AppBar(
          title: const Text('Connection Established!'),
        ),
        body: Center(
            child: Column(
                crossAxisAlignment: CrossAxisAlignment.center,
                children: <Widget> [
                  Text('Recieved: ${numReceived}',
                  style: TextStyle(fontWeight: FontWeight.bold, fontSize: 16)
                  ),
                  Text('Last Sent: ${numToSend}',
                  style: TextStyle(fontWeight: FontWeight.bold, fontSize: 16)
                  ),
                  new Container(
                    width: 200.0,
                    child: new TextField(
                      controller: numToSendCtrl,
                      autofocus: true,
                      decoration: new InputDecoration(
                        labelText: 'Enter a number',
                        fillColor: Colors.white,
                        border: new OutlineInputBorder(borderRadius: new BorderRadius.circular(10.0)),
                      ),
                    ),
                  ),
                  new RaisedButton(
                      onPressed: () => sendInt(numToSendCtrl.text),
                      child: Text('Send')
                  ),
                  new RaisedButton(
                    onPressed: () => readIntHelper(),
                    child: Text('Fetch')
                  )
                ]
            )
        )
    );
  }

  readIntHelper() {
    MyApp.device.discoverServices().then((s) {
      setState((){
        MyApp.services = s;
        MyApp.services.forEach((service) {
          for(BluetoothCharacteristic c in service.characteristics){
            readInt(c).then((value) {
              //success
            }).catchError((err){
              print(err);
            });
          }
        });
      });

    });
  }

  readInt(BluetoothCharacteristic c) async {
    final value1 = await MyApp.device.readCharacteristic(c);
    setState((){
      numReceived = value1[0];
    });
    print('TADA: ' + value1.toString());

  }
  sendInt(String txt) {
    print('INFO: Preparing to send ${numToSendCtrl.text}');
    setState(() {
      numToSend = numToSendCtrl.text;
    });
    MyApp.device.discoverServices().then((s) {
      setState((){
        MyApp.services = s;
        MyApp.services.forEach((service) {
          for(BluetoothCharacteristic c in service.characteristics){
          }
        });
      });

    });

  }
}
