#ifndef AGORA_CONFIG_ENGINE_H
#define AGORA_CONFIG_ENGINE_H
/************************************************************************/
/* IParameterEngine is used by external module such as chat engine so that
/* it can access parameters stored within the Rtc Engine
/************************************************************************/
#include <cstdint>
#include <functional>
#include <unordered_set>
#include <string>

namespace agora
{
namespace config
{

struct AnyValue {
    enum Type {
        UNSPEC = -1,
        INTEGER = 0,
        UNSIGNED_INTEGER = 1,
        BOOLEAN = 2,
        DOUBLE = 3,
        CSTR = 4,
        JSON = 5,
    }type;
    union {
        int val_int;
        unsigned int val_uint;
        bool val_bool;
        double val_double;
        const char* val_cstr;
        const void* val_cjson;
    };
};

template<class T> struct ExternalParameterHelperTypeTraits {
    static const config::AnyValue::Type AnyValueType = config::AnyValue::UNSPEC;
};
template<> struct ExternalParameterHelperTypeTraits<int> {
    static const config::AnyValue::Type AnyValueType = config::AnyValue::INTEGER;
    static int getValue(const AnyValue& value) { return value.val_int; }
    static void setValue(int from, AnyValue& to) {
        to.type = AnyValueType;
        to.val_int = from;
    }
};
template<> struct ExternalParameterHelperTypeTraits<unsigned int> {
    static const config::AnyValue::Type AnyValueType = config::AnyValue::UNSIGNED_INTEGER;
    static unsigned int getValue(const AnyValue& value) { return value.val_uint; }
    static void setValue(unsigned int from, AnyValue& to) {
        to.type = AnyValueType;
        to.val_uint = from;
    }
};
template<> struct ExternalParameterHelperTypeTraits<uint16_t> {
    static const config::AnyValue::Type AnyValueType = config::AnyValue::UNSIGNED_INTEGER;
    static uint16_t getValue(const AnyValue& value) { return value.val_uint; }
    static void setValue(uint16_t from, AnyValue& to) {
        to.type = AnyValueType;
        to.val_uint = from;
    }
};
template<> struct ExternalParameterHelperTypeTraits<double> {
    static const config::AnyValue::Type AnyValueType = config::AnyValue::DOUBLE;
    static double getValue(const AnyValue& value) { return value.val_double; }
    static void setValue(double from, AnyValue& to) {
        to.type = AnyValueType;
        to.val_double = from;
    }
};
template<> struct ExternalParameterHelperTypeTraits<bool> {
    static const config::AnyValue::Type AnyValueType = config::AnyValue::BOOLEAN;
    static bool getValue(const AnyValue& value) { return value.val_bool; }
    static void setValue(bool from, AnyValue& to) {
        to.type = AnyValueType;
        to.val_bool = from;
    }
};
template<> struct ExternalParameterHelperTypeTraits<std::string> {
    static const config::AnyValue::Type AnyValueType = config::AnyValue::CSTR;
    static std::string getValue(const AnyValue& value) { return value.val_cstr; }
    static void setValue(const std::string& from, AnyValue& to) {
        to.type = AnyValueType;
        to.val_cstr = from.c_str();
    }
};
template<> struct ExternalParameterHelperTypeTraits<const char*> {
    static const config::AnyValue::Type AnyValueType = config::AnyValue::CSTR;
    static const char* getValue(const AnyValue& value) { return value.val_cstr; }
    static void setValue(const char* from, AnyValue& to) {
        to.type = AnyValueType;
        to.val_cstr = from;
    }
};

template<> struct ExternalParameterHelperTypeTraits<const void*> {
    static const config::AnyValue::Type AnyValueType = config::AnyValue::JSON;
    static const void* getValue(const AnyValue& value) { return value.val_cjson; }
    static void setValue(const void* from, AnyValue& to) {
        to.type = AnyValueType;
        to.val_cjson = from;
    }
};

class IObserver
{
public:
    virtual bool onSetValue(const AnyValue& value) = 0;
    virtual bool onGetValue(AnyValue& value) { return false; }
};
class IFilter
{
public:
    virtual bool onSetValue(const AnyValue& value) = 0;
    virtual bool onGetValue(AnyValue& value) { return false; }
};

class IParameter
{
public:
    virtual void release() = 0;
    virtual bool getValue(AnyValue& value) const = 0;
    virtual bool setValue(const AnyValue& value, bool storeOnly) = 0;//specify storeOnly to true to skip trigger observer and filter
    virtual bool getOriginalValue(AnyValue& value) const = 0;
    virtual bool setOriginalValue(const AnyValue& value) = 0;
    virtual bool connectExternalObserver(IObserver* observer, bool triggerOnConnect) = 0;
    virtual bool disconnectExternalObserver() = 0;
    virtual bool connectExternalFilter(IFilter* filter, bool triggerOnConnect) = 0;
    virtual bool disconnectExternalFilter() = 0;
};

class IParameterCollection
{
public:
    virtual void release() = 0;
};

class IConfigEngine
{
public:
    enum PARAMETER_TYPE {
        VALUE_ONLY = 0,
        HAS_EXTERNAL_FILTER = 1,
        HAS_EXTERNAL_OBSERVER = 2,
        HAS_EXTERNAL_TRIGGER = 3,
        HAS_ORIGINAL_VALUE = 4,
    };
    virtual IParameter* createParameter(const char* key, AnyValue::Type valueType, PARAMETER_TYPE paramType) = 0;
    virtual IParameter* getParameter(const char* key) = 0;
    virtual int setParameters(const char* parameters) = 0;
};

template<class T>
class InternalParameterHelper
{
public:
    InternalParameterHelper(const T& defValue)
        :value_(defValue)
    {
    }
    const T& value() const {
        return value_;
    }
    bool setValue(const T& value) {
        value_ = value;
        return true;
    }
protected:
    T value_;
};

class ExternalTriggerParameterHelper
{
public:
    /*ExternalTriggerParameterHelper(IConfigEngine& engine, const char* key)
        :parameter_(engine.getParameter(key))
    {
    }*/
    ExternalTriggerParameterHelper(IConfigEngine& engine, const char* key, AnyValue::Type valueType, IConfigEngine::PARAMETER_TYPE paramType)
        :parameter_(engine.createParameter(key, valueType, paramType))
    {
    }
    ~ExternalTriggerParameterHelper()
    {
        if (parameter_)
            parameter_->release();
    }
    operator bool()
    {
        return parameter_ != nullptr;
    }
protected:
    IParameter* parameter_;
};

template<class T>
class ExternalParameterHelper : public ExternalTriggerParameterHelper
{
public:
    ExternalParameterHelper(IConfigEngine& engine, const char* key, const T& defValue, AnyValue::Type valueType = ExternalParameterHelperTypeTraits<T>::AnyValueType, IConfigEngine::PARAMETER_TYPE paramType = IConfigEngine::VALUE_ONLY)
        :ExternalTriggerParameterHelper(engine, key, valueType, paramType)
    {
        setValue(defValue, true);
    }
    T value() const {
        AnyValue val;
        if (parameter_ && parameter_->getValue(val)) {
            return ExternalParameterHelperTypeTraits<T>::getValue(val);
        }
        return T();
    }
    bool setValue(const AnyValue& value, bool storeValue = true) {
        if (parameter_) {
            return parameter_->setValue(value, storeValue);
        }
        return false;
    }
    bool setValue(T value, bool storeValue = true) {
        AnyValue v;
        ExternalParameterHelperTypeTraits<T>::setValue(value, v);
        return setValue(v, storeValue);
    }
};

template<class T>
class ExternalParameterHelperWithOriginalValue : public ExternalParameterHelper<T>
{
public:
    ExternalParameterHelperWithOriginalValue(IConfigEngine& engine, const char* key, const T& defValue, AnyValue::Type valueType = ExternalParameterHelperTypeTraits<T>::AnyValueType, IConfigEngine::PARAMETER_TYPE paramType = IConfigEngine::VALUE_ONLY)
        :ExternalParameterHelper<T>(engine, key, defValue, valueType, paramType)
    {
        setOriginalValue(defValue);
    }
    bool getOriginalValue(T& v) const {
        AnyValue val;
        if (this->parameter_ && this->parameter_->getOriginalValue(val)) {
            v = ExternalParameterHelperTypeTraits<T>::getValue(val);
            return true;
        }
        return false;
    }
protected:
    bool setOriginalValue(const T& value) {
        if (this->parameter_) {
            AnyValue v;
            ExternalParameterHelperTypeTraits<T>::setValue(value, v);
            return this->parameter_->setOriginalValue(v);
        }
        return false;
    }
};

class ExternalParameterHasSlots
{
public:
    virtual ~ExternalParameterHasSlots()
    {
        disconnectAll();
    }
    void disconnectAll() {
        for (auto& param : parameters_) {
            param->disconnectExternalObserver();
            param->disconnectExternalFilter();
        }
        parameters_.clear();
    }
    void addParameter(IParameter* param) {
        if (param)
            parameters_.insert(param);
    }
private:
    std::unordered_set<IParameter*> parameters_;
};

template<class T>
class ExternalParameterHelperWithObserver : public ExternalParameterHelper<T> , public IObserver
{
    typedef std::function<int(const T&)> setter_type;
    typedef std::function<int(T&)> getter_type;
public:
    ExternalParameterHelperWithObserver(IConfigEngine& engine, const char* key, const T& defValue, AnyValue::Type valueType = ExternalParameterHelperTypeTraits<T>::AnyValueType)
        :ExternalParameterHelper<T>(engine, key, defValue, valueType, IConfigEngine::HAS_EXTERNAL_OBSERVER)
    {
    }
    bool connect(ExternalParameterHasSlots* om, setter_type&& setter, getter_type&& getter = nullptr, bool triggerOnConnect = false) {
        if (this->parameter_) {
            this->setter_ = std::move(setter);
            this->getter_ = std::move(getter);
            if (om)
                om->addParameter(this->parameter_);
            return this->parameter_->connectExternalObserver(this, triggerOnConnect);
        }
        return false;
    }
    virtual bool onSetValue(const AnyValue& value) override {
        if (this->setter_) {
            return this->setter_(ExternalParameterHelperTypeTraits<T>::getValue(value)) == 0;
        }
        return false;
    }
    virtual bool onGetValue(AnyValue& value) override {
        if (this->getter_) {
            T tmp;
            if (this->getter_(tmp) == 0) {
                ExternalParameterHelperTypeTraits<T>::setValue(tmp, value);
                return true;
            }
        }
        return false;
    }
private:
    setter_type setter_;
    getter_type getter_;
};

template<class T>
class ExternalParameterHelperWithFilter : public ExternalParameterHelper<T>, public IFilter
{
    typedef std::function<int(const T&)> setter_type;
    typedef std::function<int(T&)> getter_type;
public:
    ExternalParameterHelperWithFilter(IConfigEngine& engine, const char* key, const T& defValue, AnyValue::Type valueType = ExternalParameterHelperTypeTraits<T>::AnyValueType)
        :ExternalParameterHelper<T>(engine, key, defValue, valueType, IConfigEngine::HAS_EXTERNAL_FILTER)
    {
        }
    bool connect(ExternalParameterHasSlots* om, setter_type&& setter, getter_type&& getter = nullptr, bool triggerOnConnect = false) {
        if (this->parameter_) {
            this->setter_ = std::move(setter);
            this->getter_ = std::move(getter);
            if (om)
                om->addParameter(this->parameter_);
            return this->parameter_->connectExternalFilter(this, triggerOnConnect);
        }
        return false;
    }
    virtual bool onSetValue(const AnyValue& value) override {
        return this->setter_ && this->setter_(ExternalParameterHelperTypeTraits<T>::getValue(value) == 0);
    }
    virtual bool onGetValue(AnyValue& value) override {
        if (this->getter_) {
            T tmp;
            if (this->getter_(tmp) == 0) {
                ExternalParameterHelperTypeTraits<T>::setValue(tmp, value);
                return true;
            }
        }
        return false;
    }
private:
    setter_type setter_;
    getter_type getter_;
};

template<class T>
class ExternalTriggerParameterHelperWithObserver : public ExternalTriggerParameterHelper, public IObserver
{
    typedef std::function<int(const T&)> setter_type;
    typedef std::function<int(T&)> getter_type;
public:
    ExternalTriggerParameterHelperWithObserver(IConfigEngine& engine, const char* key, AnyValue::Type valueType = ExternalParameterHelperTypeTraits<T>::AnyValueType)
        :ExternalTriggerParameterHelper(engine, key, valueType, IConfigEngine::HAS_EXTERNAL_TRIGGER)
    {
    }
    bool connect(ExternalParameterHasSlots* om, setter_type&& setter, getter_type&& getter = nullptr, bool triggerOnConnect = false) {
        if (this->parameter_) {
            this->setter_ = std::move(setter);
            this->getter_ = std::move(getter);
            if (om)
                om->addParameter(this->parameter_);
            return this->parameter_->connectExternalObserver(this, triggerOnConnect);
        }
        return false;
    }
    virtual bool onSetValue(const AnyValue& value) override {
        if (this->setter_) {
            return this->setter_(ExternalParameterHelperTypeTraits<T>::getValue(value)) == 0;
        }
        return false;
    }
    virtual bool onGetValue(AnyValue& value) override {
        if (this->getter_) {
            T tmp;
            if (this->getter_(tmp) == 0) {
                ExternalParameterHelperTypeTraits<T>::setValue(tmp, value);
                return true;
            }
        }
        return false;
    }
private:
    setter_type setter_;
    getter_type getter_;
};

} //config

} //agora

#endif //AGORA_CONFIG_ENGINE_H
