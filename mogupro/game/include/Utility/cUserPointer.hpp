#pragma once
#include <memory>
namespace Utility
{
template<class type>
using cHardPointer = std::shared_ptr<type>;
template<class type>
using hardptr = cHardPointer<type>;
// weak_ptrを使用するときに、いちいち lock とか expired を使うのがだるいのでラッピングします。
template<class type>
class cSoftPointer : public std::weak_ptr<type>
{
public:
    cSoftPointer( ) = default;
    cSoftPointer( cHardPointer<type> const& right ) noexcept
        : std::weak_ptr<type>( right )
    {
    }
    cSoftPointer( cHardPointer<type>&& right ) noexcept
        : std::weak_ptr<type>( std::move( right ) )
    {
    }
    template<class type2>
    cSoftPointer( cSoftPointer<type2> const& _Other ) noexcept
        : std::weak_ptr<type>( std::dynamic_pointer_cast<type>( _Other.lock( ) ) )
    {
    }
    template<class type2>
    cSoftPointer( cSoftPointer<type2>&& _Other ) noexcept
        : std::weak_ptr<type>( std::dynamic_pointer_cast<type>( _Other.lock( ) ) )
    {
    }
    template<class type2>
    cSoftPointer( cHardPointer<type2> const& _Other ) noexcept
        : std::weak_ptr<type>( _Other )
    {
    }
    template<class type2>
    cSoftPointer( cHardPointer<type2>&& _Other ) noexcept
        : std::weak_ptr<type>( std::move( _Other ) )
    {
    }
    template<class type2>
    cSoftPointer& operator=( cSoftPointer<type2> const& right ) noexcept
    {
        this->_Resetw( std::dynamic_pointer_cast<type>( right.lock( ) ) );
        return ( *this );
    }
    template<class type2>
    cSoftPointer& operator=( cHardPointer<type2> const& right ) noexcept
    {
        this->_Resetw( std::dynamic_pointer_cast<type>( right ) );
        return ( *this );
    }
    // ポインタの実態を返します。
    type& operator*( ) const noexcept
    {
        return *this->lock( );
    }
    // ポインタの実態を返します。
    cHardPointer<type> operator->( ) const noexcept
    {
        return this->lock( );
    }
    // ポインタがnullや期限切れを判断します。
    operator bool( ) const noexcept
    {
        return this->lock( ) && ( !this->expired( ) );
    }
    // ポインタにnullや期限切れの判断に"not"を与えます。
    bool operator!( ) const noexcept
    {
        return !( this->lock( ) && ( !this->expired( ) ) );
    }
    // ポインタ同士の比較をします。
    bool operator==( cSoftPointer<type> const& right ) const noexcept
    {
        return this->get( ) == right.get( );
    }
    // ポインタ同士の比較をします。
    bool operator==( cHardPointer<type> const& right ) const noexcept
    {
        return this->get( ) == right.get( );
    }
    // ポインタ同士の比較をします。
    bool operator!=( cSoftPointer<type> const& right ) const noexcept
    {
        return this->get( ) != right.get( );
    }
    // ポインタ同士の比較をします。
    bool operator!=( cHardPointer<type> const& right ) const noexcept
    {
        return this->get( ) != right.get( );
    }
    // ポインタ同士の比較をします。
    bool operator<( cSoftPointer<type> const& right ) const noexcept
    {
        return this->get( ) < right.get( );
    }
    // ポインタ同士の比較をします。
    bool operator<( cHardPointer<type> const& right ) const noexcept
    {
        return this->get( ) < right.get( );
    }
    // 自動的にポインタの実態を返します。
    operator type&( ) const noexcept
    {
        return *this->lock( );
    }
    // 自動的にポインタの実態を返します。
    operator type const&( ) const noexcept
    {
        return *this->lock( );
    }
    // 自動的にハードポインタにキャストします。
    operator cHardPointer<type>( ) const noexcept
    {
        return std::move( this->lock( ) );
    }
    // static_castをします。
    template<class type2>
    cHardPointer<type2> staticptr( ) const
    {
        return std::move( std::static_pointer_cast<type2>( this->lock( ) ) );
    }
    // dynamic_castをします。
    template<class type2>
    cHardPointer<type2> dynamicptr( ) const
    {
        return std::move( std::dynamic_pointer_cast<type2>( this->lock( ) ) );
    }
    // 生のポインターを返します。
    type* get( ) const noexcept
    {
        return this->lock( ).get( );
    }
};
// ポインタ同士の比較をします。
template<class type>
bool operator==( cHardPointer<type> const& left, cSoftPointer<type> const& right ) noexcept
{
    return left.get( ) == right.get( );
}
// ポインタ同士の比較をします。
template<class type>
bool operator!=( cHardPointer<type> const& left, cSoftPointer<type> const& right ) noexcept
{
    return left.get( ) != right.get( );
}
// ポインタ同士の比較をします。
template<class type>
bool operator<( cHardPointer<type> const& left, cSoftPointer<type> const& right ) noexcept
{
    return left.get( ) < right.get( );
}
template<class type>
using softptr = cSoftPointer<type>;
}