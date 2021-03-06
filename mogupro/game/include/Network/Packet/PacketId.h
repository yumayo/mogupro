#pragma once
#include <Network/NetworkDefines.h>
namespace Network
{
namespace Packet
{
enum class PacketId : ubyte1
{
    // 初期化用です。
    // 0で埋めているので、間違って呼ばれないように保険を掛けておきます。
    INVALID_NUMBER = 0U,

    //=========================================================//
    //                         Event                           //
    //=========================================================//
    // サーバー目線で書かれています。

    // <std::string>を送ります。
    EVE_STRING,
    // 接続を維持するパケットを送ります。
    EVE_PING,
    // 他のプレイヤーの位置情報を送ります。
    EVE_PLAYERS,
    // 他のプレイヤーがフィールドのジェムを採ったと通知します。
    EVE_GET_JEM_PLAYER,
    // 掘削機がフィールドのジェムを採ったと通知します。
    EVE_GET_JEM_QUARRY,
    // 他のプレイヤーや掘削機がフィールドのブロックを破壊したと通知します。
    EVE_BREAK_BLOCKS,
    // 他のプレイヤーが掘削機をフィールドに設置したと通知します。
    EVE_SET_QUARRY,
    // 他のプレイヤーに奪われたと通知します。
    // ※この瞬間にプレイヤーはステータスを変えてください。
    EVE_PLAYER_ROB_JEM,
    // 他のプレイヤーに倒されたと通知します。
    // ※この瞬間にプレイヤーはステータスを変えてください。
    EVE_PLAYER_DEATH,
	//
	EVE_RESPAWN,
	//
	EVE_ADD_CANNON_POWER,
	// 他のプレイヤーがボムを投げたと通知します。
	EVE_LIGHT_BOMB,
	//
	EVE_WEAPON_CAPSULE,
    //Memberがどこに所属しているかの送信
    EVE_TEAM_MEMBER,
	// ダメージがあったことを知らせます。
	EVE_DAMAGE,
	//
	EVE_PLAYER_ATTACK,
	//
	EVE_FRAGMENT_GEM,

    //=========================================================//
    //                        Request                          //
    //=========================================================//
    // クライアント目線で書かれています。

    // <std::string>を送ります。
    REQ_STRING,
    // サーバーに接続を申し込みます。
    REQ_CONNECT,
	//
	REQ_PING,
	//
	REQ_PLAYER,
	//
	REQ_BREAK_BLOCKS,
    // ワールドを生成するためのシード値を要求します。
    REQ_GET_JEM_SEED,
    // 現在持っているジェムの数をサーバーから取り寄せます。
    REQ_GET_JEM_POINT,
    // プレイヤーがジェムを取得したことを知らせます。
    // ※このタイミングでは取得できません。サーバーからのレスポンスを待ってください。
    REQ_GET_JEM_PLAYER,
    // 掘削機がジェムを取得したことを知らせます。
    // ※このタイミングでは取得できません。サーバーからのレスポンスを待ってください。
    REQ_GET_JEM_QUARRY,
    // プレイヤーが他のプレイヤーのジェムを奪ったことを通知します。
    // ※このタイミングでは奪取できません。サーバーからのレスポンスを待ってください。
    REQ_PLAYER_ROB_JEM,
    // 掘削機を設置したと知らせます。
    // ※このタイミングでは設置できません。サーバーからのレスポンスを待ってください。
    REQ_SET_QUARRY,
    // 他のプレイヤーを倒した情報をサーバーに送る。
    // ※このタイミングでは倒せません。サーバーからのレスポンスを待ってください。
    REQ_PLAYER_DEATH,
	// 
	REQ_RESPAWN,
	// プレイヤーがボムを投げます。
	REQ_LIGHT_BOMB,
	// 他のプレイヤーにダメージを与えたことを知らせます。
	REQ_DAMAGE,
	//Matching時にクライアントが部屋を作る申請
	REQ_MAKE_ROOM,
	//Matching時にクライアントが部屋に入る申請
	REQ_IN_ROOM,
	//Matching時にクライアントが部屋に入る申請
	REQ_IN_ROOM_WATCHING,
	//Matching時にどっちのチームに入るのかの申請
	REQ_WANT_TEAM_IN,
	//Masterからゲームが開始できるかどうかを確認するために送信
	//!@ LookMe : 受信したServerから全員に送る
	REQ_CHECK_BEGIN_GAME,
	//自分がルームにいることの送信
	REQ_CHECK_MEMBER,
	//GameMainのSetupが終わったことの通知
	REQ_END_GAMEMAIN_SETUP,
	//Game開始までのTimer終了通知
	REQ_END_START_TIMER,
	// 
	REQ_ADD_CANNON_POWER,
	//
	REQ_WEAPON_CAPSULE,
	// 
	REQ_RESULT,
	//
	REQ_PLAYER_ATTACK,
	//
	REQ_FRAGMENT_GEM,
	//
	REQ_GAME_END,

    //=========================================================//
    //                        Response                         //
    //=========================================================//
    // サーバー目線で書かれています。

    // <std::string>を送ります。
    RES_STRING,
    // クライアントの接続要請に答えます。
    RES_CONNECT,
    // ワールドを生成するためのシード値を渡します。
    RES_GET_JEM_SEED,
    // プレイヤーが持っているジェムの数を渡します。
    RES_GET_JEM_POINT,
	//Matching時にクライアントが部屋を作る申請をうけ、作れたかどうか
	RES_MAKE_ROOM,
	//Matching時にクライアントが部屋に入る申請をうけ、入れたかどうか
	RES_IN_ROOM,
	//Matching時にどっちのチームに入るのかの申請をうけ、入れたかどうか
	RES_WANT_TEAM_IN,
    //ゲームを開始できるかどうか
	RES_CHECK_BEGIN_GAME,
	//自分がルームにいることの送信してもらう
	RES_CHECK_MEMBER,
	//GameMainのSetupが終わったことの通知を受けそれに対しServerAdapterのUpdate開始を通知するかも用
	RES_END_GAMEMAIN_SETUP,
	//GameMainのSetupが終了した時点からの
	RES_SET_GAMESTART_TIMER,
	//
	RES_RESULT,
};
}
}