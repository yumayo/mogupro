public:
    boost::optional<Packet::_TYPE_::c_TEMPLATE_VALUE_> get_TEMPLATE_VALUE_( );
    void unget_TEMPLATE_VALUE_( Packet::_TYPE_::c_TEMPLATE_VALUE_&& data );
    bool isNew_TEMPLATE_VALUE_( Packet::PacketHeader const& header );
private:
    std::stack<Packet::_TYPE_::c_TEMPLATE_VALUE_> m_TEMPLATE_VALUE_;
    std::map<ubyte4, double> m_TEMPLATE_VALUE_SequenceIds;
