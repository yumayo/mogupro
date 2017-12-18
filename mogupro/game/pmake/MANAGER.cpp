		case Network::Packet::PacketId::_TEMPLATE_ENUM_:
		{
			if ( _TEMPLATE_VALUE_.isNew( packetHeader ) )
			{
				Packet::_PACKET_TYPE_::c_TEMPLATE_VALUE_ data;
				data.onReceive( networkHandle, bufferSize, bufferData );
				_TEMPLATE_VALUE_.unget( std::move( data ) );
			}
			break;
		}
