/**
  ******************************************************************************
  * @file    ebox_Template.h
  * @author  cat_li
  * @version V2.0
  * @date    2016/10/26
  * @brief   
  ******************************************************************************
  * @attention
  *
  * No part of this software may be used for any commercial activities by any form 
  * or means, without the prior written consent of shentq. This specification is 
  * preliminary and is subject to change at any time without notice. shentq assumes
  * no responsibility for any errors contained herein.
  * <h2><center>&copy; Copyright 2015 shentq. All Rights Reserved.</center></h2>
  ******************************************************************************
  */
#ifndef __TEMPLATE_H_
#define __TEMPLATE_H_

#include <string.h>
#include <stdint.h>

/** A class for storing and calling a pointer to a static or member function  �������澲̬���Ա������ָ����
 */
// R-����ֵ����  A1-��������
template <typename R, typename A1>
class FunctionPointerArg1{
public:
    /** Create a FunctionPointer, attaching a static function ��������ָ��
     *
     *  @param function The static function to attach (default is none) ���Ӿ�̬������Ĭ��Ϊvoid
     */
    FunctionPointerArg1(R (*function)(A1) = 0) {
        attach(function);
    }

    /** Create a FunctionPointer, attaching a member function
     *  ���ӳ�Ա������object ��Ա�����Ķ����ָ�롣 ��Ա����
     *  @param object The object pointer to invoke the member function on (i.e. the this pointer)
     *  @param function The address of the member function to attach
     */
    template<typename T>
    FunctionPointerArg1(T *object, R (T::*member)(A1)) {
        attach(object, member);
    }

    /** Attach a static function
     *  ������̬����
     *  @param function The static function to attach (default is none)
     */
    void attach(R (*function)(A1)) {
        _p.function = function;
        _membercaller = 0;
    }

    /** Attach a member function
     *  ������Ա����
     *  @param object The object pointer to invoke the member function on (i.e. the this pointer)
     *  @param function The address of the member function to attach
     */
    template<typename T>
    void attach(T *object, R (T::*member)(A1)) {
        _p.object = static_cast<void*>(object); // ������ת����void* ����
        *reinterpret_cast<R (T::**)(A1)>(_member) = member; 
        _membercaller = &FunctionPointerArg1::membercaller<T>; //ע���Ա������ַ
    }

    /** Call the attached static or member function
     */
    R call(A1 a) {
        if (_membercaller == 0 && _p.function) {
           return _p.function(a);
        } else if (_membercaller && _p.object) {
           return _membercaller(_p.object, _member, a);
        }
        return (R)0;
    }

    /** Get registered static function
     */
    R(*get_function(A1))() {
        return _membercaller ? (R(*)(A1))0 : (R(*)(A1))_p.function;
    }

#ifdef MBED_OPERATORS
    R operator ()(A1 a) {
        return call(a);
    }
    operator bool(void) const {
        return (_membercaller != NULL ? _p.object : (void*)_p.function) != NULL;
    }
#endif
private:
    template<typename T>  //��������
    // ���ó�Ա ���󣬳�Ա����
    static R membercaller(void *object, uintptr_t *member, A1 a) {
        T* o = static_cast<T*>(object); //����ת��
        R (T::**m)(A1) = reinterpret_cast<R (T::**)(A1)>(member);
        return (o->**m)(a);
    }

    union {
        R (*function)(A1); // static function pointer  ��̬����ָ��
        void *object;      // object this pointer      ����ָ��
    } _p;  // �������屣��ָ�룬��̬�������߶���ֻ�ܱ�������һ��
    uintptr_t _member[4]; // aligned raw member function pointer storage - converted back by registered _membercaller
    // ����ָ��
    R (*_membercaller)(void*, uintptr_t*, A1); // registered membercaller function to convert back and call _m.member on _object
};

/** A class for storing and calling a pointer to a static or member function (R ()(void))
 * �޲���
 */
template <typename R>
class FunctionPointerArg1<R, void>{
public:
    /** Create a FunctionPointer, attaching a static function
     *
     *  @param function The static function to attach (default is none)
     */
    FunctionPointerArg1(R (*function)(void) = 0) {
        attach(function);
    }

    /** Create a FunctionPointer, attaching a member function
     *
     *  @param object The object pointer to invoke the member function on (i.e. the this pointer)
     *  @param function The address of the void member function to attach
     */
    template<typename T>
    FunctionPointerArg1(T *object, R (T::*member)(void)) {
        attach(object, member);
    }

    /** Attach a static function
     *
     *  @param function The void static function to attach (default is none)
     */
    void attach(R (*function)(void)) {
        _p.function = function;
        _membercaller = 0;
    }

    /** Attach a member function
     *
     *  @param object The object pointer to invoke the member function on (i.e. the this pointer)
     *  @param function The address of the void member function to attach
     */
    template<typename T>
    void attach(T *object, R (T::*member)(void)) {
        _p.object = static_cast<void*>(object);
        *reinterpret_cast<R (T::**)(void)>(_member) = member;
        _membercaller = &FunctionPointerArg1::membercaller<T>;
    }

    /** Call the attached static or member function
     */
    R call(){
        if (_membercaller == 0 && _p.function) {
            return _p.function();
        } else if (_membercaller && _p.object) {
            return _membercaller(_p.object, _member);
        }
        return (R)0;
    }

    /** Get registered static function
     */
    R(*get_function())() {
        return _membercaller ? (R(*)())0 : (R(*)())_p.function;
    }

private:
    template<typename T>
    static R membercaller(void *object, uintptr_t *member) {
        T* o = static_cast<T*>(object);
        R (T::**m)(void) = reinterpret_cast<R (T::**)(void)>(member);
        return (o->**m)();
    }

    union {
        R (*function)(void); // static function pointer
        void *object;        // object this pointer
    } _p;
    uintptr_t _member[4]; // aligned raw member function pointer storage - converted back by registered _membercaller
    R (*_membercaller)(void*, uintptr_t*); // registered membercaller function to convert back and call _m.member on _object
};

typedef FunctionPointerArg1<void, void> FunctionPointer;  // �޲κ���ָ��
typedef FunctionPointerArg1<void, int>  event_callback_t; // �¼��ص�����

#endif
