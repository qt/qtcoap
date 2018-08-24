## Introduction
This is the Qt CoAP module repository. CoAP is a protocol for IoT devices, and machine to machine communication.
The full specification can be found in [RFC 7252](https://tools.ietf.org/html/rfc7252).

### Supported features

- CoAP Client
- Send GET/POST/PUT/DELETE requests
- Discover resources (single server)
- Observe resources and cancel the observation
- Blockwise requests and replies
- Confirmable and non-confirmable messages
- Some options can be added to the request
- Replies can be received in a separate or piggybacked message

### Unsupported yet

- Multicast discovery
- DTLS
- CoAP Server

## How to use the library

### GET/POST/PUT/DELETE requests
```c++
QCoapClient* client = new QCoapClient(this);
connect(client, &QCoapClient::finished, this, &MyClass::onFinished);
client->get(QUrl("coap://coap.me/test"));
client->put(QUrl("coap://coap.me/test"), QByteArray("payload"));
```
or
```c++
QCoapReply* reply = client->get(QCoapRequest("coap://coap.me/test"));
connect(reply, &QCoapReply::finished, this, &MyClass::onFinished);
```
The slot connected to the `QCoapReply::finished(QCoapReply *)` signal can use the `QCoapReply` object like a `QIODevice` object.

### OBSERVE requests
Observe requests are used to receive automatic server notifications for a resource. For Observe requests specifically, you can use the `QCoapReply::notified(QCoapReply *, QCoapMessage)` signal to handle notifications from the CoAP server.
```c++
QCoapRequest request = QCoapRequest("coap://coap.me/obs");
QCoapReply* reply = client->observe(request);
connect(reply, &QCoapReply::notified, this, &MyClass::onNotified);
```

You can then stop the observation with
```c++
client->cancelObserve(reply);
```

The notified signal will provide the `QCoapReply` and most recent message.

### DISCOVERY requests
For machine to machine communication, CoAP Discovery requests is used to query the resources available to an endpoint, or to the complete network.
```c++
QCoapDiscoveryReply* reply = client->discover("coap://coap.me/");
connect(reply, &QCoapReply::discovered, this, &MyClass::onDiscovered);
```

The signal `discovered` can be triggered multiple times, and will provide the list of resources returns by the server(s).

## Automated tests
Automated tests require a Californium plugtest server. Plugtest is a CoAP server used to test the main features of the CoAP protocol.
The following command starts a plugtest server using Docker.

```bash
docker run --name coap-test-server -d --rm -p 5683:5683/udp aleravat/coap-test-server:latest
```

Automated tests require `COAP_TEST_SERVER_IP` environment variable to be set, containing Plugtest server IP address. This address will be used to connect to the Plugtest server on port 5683.

The IP address of the docker container can found identified by:
1. Retrieve the container id with `docker ps`
```
$ docker ps
CONTAINER ID        IMAGE                                           [...]
826073e84e7f        aleravat/coap-test-server:latest                [...]
```
2. Get the IP address with `docker inspect <container_id> | grep IPAddress`
```
$ docker inspect 826073e84e7f | grep IPAddress
[...]
        "IPAddress": "172.17.0.3",
[...]
```
3. Set the environment variable in QtCreator, or in a the terminal used `export COAP_TEST_SERVER_IP="172.17.0.3"`
