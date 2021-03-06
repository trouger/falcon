
#ifndef FALCON_REGISTER_H
#define FALCON_REGISTER_H

#include "py_include.h"

#include "config.h"
#include "inline.h"

static const int ObjType = 0;
static const int IntType = 1;

#if USE_TYPED_REGISTERS

#define TYPE_MASK 0x1

struct Register {
  union {
    int64_t i_value;
    double f_value;
    PyObject* objval;
  };

  Register() {

  }

  Register(PyObject* v) {
    store(v);
  }

  Register(const Register& r) {
    objval = r.objval;
  }

  operator long() {
    return as_int();
  }

  operator PyObject*() {
    return as_obj();
  }

  int compare(PyObject* v, int *result) {
    if (PyInt_Check(v) && get_type() == IntType) {
      long lv = PyInt_AsLong(v);
      long rv = long(*this);
      if (rv > lv) { *result = 1; }
      else if (rv == lv) { *result = 0; }
      else { *result = -1; }
      return 0;
    }

    return PyObject_Cmp((PyObject*)this, v, result);
  }

  f_inline long as_int() const {
//    Log_Info("load: %p : %d", this, value);
    return i_value >> 1;
  }

  f_inline bool is_obj() {
    return get_type() == ObjType;
  }

  f_inline PyObject* as_obj() {
    if (get_type() == ObjType) {
      return objval;
    } else {
      objval = PyInt_FromLong(as_int());
      //Log_Info("Coerced: %p %d", this, objval->ob_refcnt);
      return objval;
    }
  }

  f_inline void reset() {
    objval = (PyObject*) NULL;
  }

  f_inline int get_type() const {
    return (i_value & TYPE_MASK);
  }

  f_inline void decref() {
    if (get_type() == ObjType) {
//      Log_Info("Decref %p %d", this, objval == NULL ? -1 : objval->ob_refcnt);
      Py_XDECREF(objval);
    }
  }

  f_inline void incref() {
    if (get_type() == ObjType) {
//      Log_Info("Incref %p %d", this, objval == NULL ? -1 : objval->ob_refcnt);
      Py_INCREF(objval);
    }
  }

  template<bool DECREF_OLD = false>
  f_inline void store(Register& r) {
      if (DECREF_OLD && get_type() == ObjType) {
          auto old = objval;
          objval = r.objval;
          Py_XDECREF(old);
      } else 
          objval = r.objval;
  }

  template<bool DECREF_OLD = false>
  f_inline void store(int v) {
    store<DECREF_OLD>((long) v);
  }

  template<bool DECREF_OLD = false>
  f_inline void store(long v) {
//    Log_Info("store: %p : %d", this, v);
      if (DECREF_OLD && get_type() == ObjType) {
          auto old = objval;
          i_value = v << 1;
          i_value |= IntType;
          Py_XDECREF(old);
      }
      else {
          i_value = v << 1;
          i_value |= IntType;
      }
  }

  template<bool DECREF_OLD = false>
  f_inline void store(PyObject* obj) {
    if (obj == NULL || !PyInt_CheckExact(obj)) {
        if (DECREF_OLD) {
            auto old = objval;
            objval = obj;
            Py_XDECREF(old);
        } else 
            // Type flag is implicitly set to zero as a result of pointer alignment.
            objval = obj;
    } else {
      store<DECREF_OLD>(PyInt_AS_LONG(obj) );
      Py_DECREF(obj);
      //Log_Info("Register store object %d %d", as_int(), obj->ob_refcnt);
    }
  }
};

#else
struct Register {
  PyObject* v;

  Register() {}
  Register(PyObject* o) : v(o) {}

  f_inline int get_type() {
    if (PyInt_CheckExact(v)) {
      return IntType;
    } else {
      return ObjType;
    }
  }
  
  int compare(PyObject* v, int *result) {
    return PyObject_Cmp((PyObject*)this, v, result);
  }

  f_inline bool is_obj() {
    return true;
  }

  f_inline PyObject*& as_obj() {
    return v;
  }

  f_inline long as_int() {
    return PyInt_AsLong(v);
  }

  f_inline void decref() {
    Py_XDECREF(v);
  }

  f_inline void incref() {
    Py_INCREF(v);
  }

  f_inline void reset() {
    v = (PyObject*) NULL;
  }

  template<bool DECREF_OLD = false>
  f_inline void store(PyObject* obj) {
      if (DECREF_OLD) {
          auto old = v;
          v = obj;
          Py_XDECREF(old);
      } else {
          v = obj;
      }    
  }

  template<bool DECREF_OLD = false>
  f_inline void store(Register& r) {
      if (DECREF_OLD) {
          auto old = v;
          v = r.v;
          Py_XDECREF(old);
      }
      else {
          v = r.v;
      }
  }

  template<bool DECREF_OLD = false>
  f_inline void store(int ival) {
    store<DECREF_OLD>((long)ival);
  }

  template<bool DECREF_OLD = false>
  f_inline void store(long ival) {
    if (DECREF_OLD) {
        auto old = v;
        v = PyInt_FromLong(ival);
        Py_XDECREF(old);
    }
    else {
        v = PyInt_FromLong(ival);
    }
  }
};
#endif

#endif
