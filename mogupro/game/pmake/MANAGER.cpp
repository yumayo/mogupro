boost::optional<Packet::_TYPE_::c_TEMPLATE_VALUE_> c_TYPE_Manager::get_TEMPLATE_VALUE_( )
{
    if ( m_TEMPLATE_VALUE_.empty( ) )
    {
        auto it = m_TEMPLATE_VALUE_SequenceIds.begin( );
		while ( it != m_TEMPLATE_VALUE_SequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - 5.0F )
			{
				m_TEMPLATE_VALUE_SequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = m_TEMPLATE_VALUE_.top( );
        m_TEMPLATE_VALUE_.pop( );
        return top;
    }
}
void c_TYPE_Manager::unget_TEMPLATE_VALUE_( Packet::_TYPE_::c_TEMPLATE_VALUE_&& data )
{
    m_TEMPLATE_VALUE_.push( std::move( data ) );
}
bool c_TYPE_Manager::isNew_TEMPLATE_VALUE_( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
	auto status = m_TEMPLATE_VALUE_SequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
	return status.second;
}
