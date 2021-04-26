#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MQTTClient.h"

#define ADDRESS     "tcp://broker.emqx.io:1883"
#define CLIENTID    "I_am_the_best11"
//#define TOPIC       "/dubesundaram99@gmail.com/sundaram"
//#define TOPIC1       "/ravi123sehravat@gmail.com/narendra"//subscribing to this
#define TOPIC       "/dubesundaram99@gmail.com/sundaram" // publishing to this
//#define TOPIC       "/kajupretty2202@gmail.com/kaju"//  publishing to this
#define PAYLOAD     "Hello World!"
#define QOS         1
#define TIMEOUT     10000L

volatile MQTTClient_deliveryToken deliveredtoken;

void delivered(void *context, MQTTClient_deliveryToken dt)
{
    printf("Message with token value %d delivery confirmed\n", dt);
    deliveredtoken = dt;
}

// int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
// {
//     //printf("Message arrived\n");
//     //printf("   subscribing to  topic: %s\n", topicName);
//     printf("                           ravi sent: %.*s\n", message->payloadlen, (char*)message->payload);
//     MQTTClient_freeMessage(&message);
//     MQTTClient_free(topicName);
//     return 1;
// }

void connlost(void *context, char *cause)
{
    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);
}

int main(int argc, char* argv[])
{
    

// publishing vala
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
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   
    char str[100];
    
    
    while(strcmp(str, "quit")!=0)
    {
        //printf("string is %c\t:" str[0]);
        pubmsg.payload = str;
        pubmsg.payloadlen = (int)strlen(str);
        pubmsg.qos = QOS;
        pubmsg.retained = 0;
        MQTTClient_publishMessage(client1, TOPIC, &pubmsg, &token);
        //printf("type: \n");
        fgets(str,100,stdin);
        //printf("Sundaram: %s",&str[0]);
        
        if(str[0]=='q') break;//for exiting console
    }
    //
    // if ((rc = MQTTClient_publishMessage(client1, TOPIC, &pubmsg, &token)) != MQTTCLIENT_SUCCESS)
    // {
    //      printf("Failed to publish message, return code %d\n", rc);
    //      exit(EXIT_FAILURE);
    // }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    printf("Waiting for up to %d seconds for publication of %s\n"
            "on topic %s for client with ClientID: %s\n",
            (int)(TIMEOUT/1000), PAYLOAD, TOPIC, CLIENTID);
    rc1 = MQTTClient_waitForCompletion(client1, token, TIMEOUT);
    printf("Message with delivery token %d delivered\n", token);

    // if ((rc = MQTTClient_disconnect(client, 10000)) != MQTTCLIENT_SUCCESS)
    //     printf("Failed to disconnect, return code %d\n", rc);
    // MQTTClient_destroy(&client);





    // if ((rc = MQTTClient_disconnect(client, 10000)) != MQTTCLIENT_SUCCESS)
    // {
    //     printf("Failed to disconnect, return code %d\n", rc);
    //     rc = EXIT_FAILURE;
    // }

    return rc1;
    // publishing vala***********************************************************************************************************
}


//        int ch;
//         do
//         {
//             ch = getchar();
//         } while (ch!='Q' && ch != 'q');