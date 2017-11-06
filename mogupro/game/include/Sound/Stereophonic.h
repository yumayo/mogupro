//#pragma once
//#include <Sound/Wav.h>
////! @fn    Stereophonic
////! @brief ���̉������Đ�����Class�ł�
////! @note  �g�����ɂ���
////!
////!                1.read 
////!                2.bind 
////!                3.setListenerPos
////!                4.setListenerDirection
////!                5.setSourcePos
////!                6.play�Ƃ�
////!
////! @date 2017-11-06
////! @author Taka Nomoto
//
//namespace Sound
//{
//	class Stereophonic
//	{
//	public:
//		//Assets��Path�w�肵�Ă�������
//		//Wav�ǂݍ���
//		void read(const std::string& file);
//		//���C���X���b�h�Ńf�[�^��Bind���s��
//		void bind();
//		//�����ʒu�̐ݒ�
//		void setListenerPos(const float& x, const float& y, const float& z);
//		//���������̐ݒ� (up�x�N�g����vec3(0,1,0))
//		void setListenerDirection(const float& x, const float& y, const float& z);
//		//�����̈ʒu�̐ݒ�
//		void setSourcePos(const float& x, const float& y, const float& z);
//		
//		void play();
//		
//		void stop();
//		
//		void pause();
//		
//		void gain(const float value);
//		
//		void pitch(const float value);
//		
//		void looping(const bool value);
//		
//		bool isPlaying();
//		
//		float currentTime();
//		
//		void clean();
//
//	private:
//		Wav wav;
//		ALCdevice*  device;
//		ALCcontext* context;
//
//		ALuint bufferId;
//		ALuint sourceId;
//	};
//}