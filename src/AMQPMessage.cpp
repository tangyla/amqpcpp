/*
 *  AMQPMessage.cpp
 *  librabbitmq++
 *
 *  Created by Alexandre Kalendarev on 15.04.10.
 *
 */

#include "AMQPcpp.h"

AMQPMessage::AMQPMessage( AMQPQueue * queue )
    :data_(NULL),
     len_(-1),
     delivery_tag_(0),
     message_count_(-1),
     queue_(queue) {
}

AMQPMessage::~AMQPMessage() {
    if (data_) {
        free(data_);
        data_ = NULL;
    }
}

void AMQPMessage::setMessage(const char * data, uint32_t length) {
    if (data && length) {
        if (data_) {
            free(data_);
            data_ = NULL;
        }

        data_ = (char*)malloc(length);
        memcpy(data_, data, length);
        len_ = length;
    }
}

char * AMQPMessage::getMessage(uint32_t* length) {
    if (data_) {
        *length = len_;
        return data_;
    }

    *length = 0;

    return '\0';
}

string AMQPMessage::getConsumerTag() {
    return consumer_tag_;
}

void AMQPMessage::setConsumerTag(amqp_bytes_t consumer_tag) {
    consumer_tag_.assign( (char*)consumer_tag.bytes, consumer_tag.len);
}

void AMQPMessage::setConsumerTag(string consumer_tag) {
    consumer_tag_ = consumer_tag;
}

void AMQPMessage::setDeliveryTag(uint32_t delivery_tag) {
    delivery_tag_ = delivery_tag_;
}

uint32_t AMQPMessage::getDeliveryTag() {
    return delivery_tag_;
}

void AMQPMessage::setMessageCount(int count) {
    message_count_ = count;
}

int AMQPMessage::getMessageCount() {
    return message_count_;
}

void AMQPMessage::setExchange(amqp_bytes_t exchange) {
    if (exchange.len)
        exchange_.assign( (char*)exchange.bytes, exchange.len );
}

void AMQPMessage::setExchange(string exchange) {
    exchange_ = exchange;
}

string AMQPMessage::getExchange() {
    return exchange_;
}

void AMQPMessage::setRoutingKey(amqp_bytes_t routing_key) {
    if (routing_key.len)
        routing_key_.assign( (char*)routing_key.bytes, routing_key.len );
}

void AMQPMessage::setRoutingKey(string routing_key) {
    routing_key_ = routing_key;
}

string AMQPMessage::getRoutingKey() {
    return routing_key_;
}

void AMQPMessage::addHeader(const char * name, amqp_boolean_t * value) {
    headers_[name] = string(*value? "false":"true");
}

void AMQPMessage::addHeader(const char * name, uint64_t * value) {
    char ivalue[32] = {0};
    sprintf(ivalue, "%lu", *value);
    headers_[name] = string(ivalue);
}

void AMQPMessage::addHeader(const char * name, uint8_t * value) {
    char ivalue[4] = {0};
    sprintf(ivalue, "%d", *value);
    headers_[name] = string(ivalue);
}

void AMQPMessage::addHeader(const char * name, double * value) {
    char dvalue[16] = {0};
    sprintf(dvalue, "%.02f", *value);
    headers_[name] = string(dvalue);
}

void AMQPMessage::addHeader(const char* name, amqp_bytes_t * value) {
    string svalue;
    svalue.assign(( const char *) value->bytes, value->len);
    headers_[name] = svalue;
}

void AMQPMessage::addHeader(amqp_bytes_t * name, amqp_bytes_t * value) {
    string sname, svalue;
    sname.assign((const char *) name->bytes, name->len);
    svalue.assign((const char *) value->bytes, value->len);

    headers_[sname] = string(svalue);
}

string AMQPMessage::getHeader(string name) {
    if (headers_.find(name) == headers_.end())
        return "";
    else
        return headers_[name];
}

AMQPQueue * AMQPMessage::getQueue() {
    return queue_;
}
