boost::optional<Packet::_TYPE_::c_TEMPLATE_VALUE_> c_TYPE_Manager::get_TEMPLATE_VALUE_( )
{
    if ( m_TEMPLATE_VALUE_.empty( ) )
    {
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
