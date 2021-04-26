#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "MQTTClient.h"

#define ADDRESS     "tcp://broker.emqx.io:1883"
#define CLIENTID    "I_am_the_best2"

#define TOPIC       "/ravi123sehravat@gmail.com/narendra"// publishing to this

#define PAYLOAD     "Hello World!"
#define QOS         1
#define TIMEOUT     10000L

volatile MQTTClient_deliveryToken deliveredtoken;

void delivered(void *context, MQTTClient_deliveryToken dt)
{
    printf("Message with token value %d delivery confirmed\n", dt);
    deliveredtoken = dt;
}

void connlost(void *context, char *cause)
{
    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);
}

int main(int argc, char* argv[])
{
   
    MQTTClient client1;
    MQTTClient_connectOptions conn_opts1 = MQTTClient_connectOptions_initializer;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    int rc1;


    if ((rc1 = MQTTClient_create(&client1, ADDRESS, CLIENTID,
        MQTTCLIENT_PERSISTENCE_NONE, NULL)) != MQTTCLIENT_SUCCESS)
    {
         printf("Failed to create client, return code %d\n", rc1);
         exit(EXIT_FAILURE);
    }

    //conn_opts1.keepAliveInterval = 20;
    conn_opts1.keepAliveInterval = 100;
    conn_opts1.cleansession = 1;
    if ((rc1 = MQTTClient_connect(client1, &conn_opts1)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc1);
        exit(EXIT_FAILURE);
    }

    char str[100];
    
    while(strcmp(str, "quit")!=0)
    {
        
        pubmsg.payload = str;
        pubmsg.payloadlen = (int)strlen(str);
        pubmsg.qos = QOS;
        pubmsg.retained = 0;
        
        
        MQTTClient_publishMessage(client1, TOPIC, &pubmsg, &token);
        
        fgets(str,100,stdin);
        //printf("Narendra: %s",&str[0]);
        
        if(str[0]=='q') break;//for exiting console

    }
   
    printf("Waiting for up to %d seconds for publication of %s\n"
            "on topic %s for client with ClientID: %s\n",
            (int)(TIMEOUT/1000), PAYLOAD, TOPIC, CLIENTID);
    rc1 = MQTTClient_waitForCompletion(client1, token, TIMEOUT);
    printf("Message with delivery token %d delivered\n", token);

    return rc1;

}
