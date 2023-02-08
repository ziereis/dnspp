//
// Created by thomas on 04.02.23.
//

#include <ostream>
#include <utility>
#include "DnsPacket.h"
#include "BufferParser.h"

namespace Dns
{
    DnsPacket::DnsPacket(const std::array<uint8_t, DNS_BUF_SIZE>& buf, size_t bytes_read)
    : header_{}, questions{}, answers{}, authorities{}, additionals{}
    {
        BufferParser parser{std::span(buf.data(), bytes_read)};
        header_ = parser.read_header();
        LOG(header_);

        for(size_t i = 0; i < header_.question_count; i++)
            questions.emplace_back(parser.read_question());

        for(size_t i = 0; i < header_.answer_count; i++)
            answers.emplace_back(parser.read_answer());

        for(size_t i = 0; i < header_.authority_count; i++)
            authorities.emplace_back(parser.read_answer());

        for(size_t i = 0; i < header_.addtional_count; i++)
            additionals.emplace_back(parser.read_answer());

#if ENABLE_DEBUG_LOG
        for(auto& q : questions)
            std::cout << q << std::endl;
        for(auto& q : answers)
            std::cout << q << std::endl;
        for(auto& q : authorities)
            std::cout << q << std::endl;
        for(auto& q : additionals)
            std::cout << q << std::endl;
#endif


    }


    QueryType  get_query_type(uint16_t query_num) {
        switch (query_num) {
            case static_cast<uint16_t>(QueryType::A):
                return QueryType::A;
            case static_cast<uint16_t>(QueryType::AAA):
                return QueryType::AAA;
            case static_cast<uint16_t>(QueryType::NS):
                return QueryType::NS;
            case static_cast<uint16_t>(QueryType::CNAME):
                return QueryType::CNAME;
            case static_cast<uint16_t>(QueryType::MX):
                return QueryType::MX;
            default:
                return QueryType::UNKNOWN;
        }
    }
    std::ostream& operator<<(std::ostream &os, const DnsHeader &header) {
        os << "DnsHeader{"
           << "\n\tid: " << header.id
/*
           << "\n\tquery_response: " << std::to_string(header.query_response)
           << "\n\top_code: " << std::to_string(header.op_code)
           << "\n\tauthoritative_answer: " << std::to_string(header.authoritative_answer)
           << "\n\ttruncated_message: " << std::to_string(header.truncated_message)
           << "\n\trecursion_desired: " << std::to_string(header.recursion_desired)
           << "\n\trecursion_available: " << std::to_string(header.recursion_available)
           << "\n\treserved: " << std::to_string(header.reserved)
           << "\n\tresponse_code: " << std::to_string(header.response_code)
*/
            << "\n\t flags1 =  :" << std::bitset<8>(header.flags1)
            << "\n\t flags2 =  :" << std::bitset<8>(header.flags2)
           << "\n\tquestion_count: " << header.question_count
           << "\n\tanswer_count: " << header.answer_count
           << "\n\tauthority_count: " << header.authority_count
           << "\n\taddtional_count: " << header.addtional_count << "}";
        return os;
    }


    uint8_t DnsHeader::get_query_response() {
        return 0;
    }

    uint8_t DnsHeader::get_op_code() {
        return 0;
    }

    uint8_t DnsHeader::get_authoritative_answer() {
        return 0;
    }

    uint8_t DnsHeader::get_truncated_message() {
        return 0;
    }

    uint8_t DnsHeader::get_recursion_desired() {
        return 0;
    }

    uint8_t DnsHeader::get_recursion_available() {
        return 0;
    }

    u_int8_t DnsHeader::get_reserved() {
        return 0;
    }

    u_int8_t DnsHeader::get_response_code() {
        return 0;
    }

    std::ostream& operator<<(std::ostream &os, const DnsQuestion &question) {
        os << "name: " << question.name << " query_type: "
        << question.query_type << " query_class: "
        << question.query_class;
        return os;
    }


    std::ostream &operator<<(std::ostream &os, const DnsAnswer &answer) {
        os << "name: " << answer.name << " query_type: " << static_cast<uint16_t>(answer.query_type) << " query_class: " << answer.query_class
           << " ttl: " << answer.ttl << " len: " << answer.len << " record: ";

        std::visit(RecordPrintVisitor{}, answer.record);
        return os;
    }

    DnsAnswer::DnsAnswer(std::string name, QueryType queryType, uint16_t query_class,
                         uint32_t ttl, uint16_t len, DnsRecord record)
                         : name{std::move(name)}
                         , query_type{queryType}
                         , query_class{query_class}
                         , ttl{ttl}
                         , len{len}
                         , record{std::move(record)}
                         {}
}
