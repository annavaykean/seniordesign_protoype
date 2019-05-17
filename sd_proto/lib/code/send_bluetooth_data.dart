import 'package:flutter/material.dart';
import 'package:flutter_blue/flutter_blue.dart';

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
        body: Column(
            children: <Widget> [
              Text('Recieved: ${numReceived}'),
              Text('Last Sent: ${numToSend}'),
              new TextField(
                controller: numToSendCtrl,
                autofocus: true,
                decoration: new InputDecoration(
                  labelText: 'Enter a number',
                  fillColor: Colors.white,
                  border: new OutlineInputBorder(borderRadius: new BorderRadius.circular(25.0)),
                ),
              ),
              new RaisedButton(
                  onPressed: () => sendInt(numToSendCtrl.text),
                  child: Text('Send')
              )
            ]
        )
    );
  }

  sendInt(String txt) {
    print('INFO: Preparing to send ${numToSendCtrl.text}');
    setState(() {
      numToSend = numToSendCtrl.text;
    });
  }
}
