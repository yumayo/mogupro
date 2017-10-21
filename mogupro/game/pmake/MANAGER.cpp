Packet::_TYPE_::c_TEMPLATE_VALUE_ c_TYPE_Manager::get_TEMPLATE_VALUE_( )
{
    auto top = m_TEMPLATE_VALUE_.top( );
    m_TEMPLATE_VALUE_.pop( );
    return std::move( top );
}
void c_TYPE_Manager::unget_TEMPLATE_VALUE_( Packet::_TYPE_::c_TEMPLATE_VALUE_ const & data )
{
    m_TEMPLATE_VALUE_.push( data );
}
