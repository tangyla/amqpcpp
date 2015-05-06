#include <unistd.h>
#include "AMQPcpp.h"

int main (int argc, char** argv) {
    try {
        AMQP amqp("123456:admin@127.0.0.1:5672//");

        string exchange_name("amq.topic");
        string exchange_type("topic");
        AMQPExchange* exchange = amqp.createExchange(exchange_name);
        exchange->Declare(exchange_name, exchange_type, AMQP_DURABLE);

        string queue_name("test");
        AMQPQueue * qu2 = amqp.createQueue(queue_name);
        qu2->Declare();
        qu2->Bind(exchange_name, queue_name);

        int i = 0;
        while (1) {
            qu2->Get(AMQP_NOACK);
            AMQPMessage * m= qu2->getMessage();
            cout << ++i << "th, "<< "get message count: " <<  m->getMessageCount() + 1 << "." << endl;
            if (m->getMessageCount() > -1) {
                uint32_t j = 0;
                cout << "message\n";
                cout << "content:"<< m->getMessage(&j) << endl;
                cout << "key: "<< m->getRoutingKey() << endl;
                cout << "exchange: "<< m->getExchange() << endl;
                cout << "Content-type: "<< m->getHeader("Content-type") << endl;
                cout << "Content-encoding: "<< m->getHeader("Content-encoding") << endl;
                cout << "\n";
            }
            else {
                sleep(60);
            }
        }
    } catch (AMQPException e) {
        std::cout << e.getMessage() << std::endl;
    }

    return 0;
}
