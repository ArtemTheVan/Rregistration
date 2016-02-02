#ifndef INTERFACE_SINGLETON_H
#define INTERFACE_SINGLETON_H


#ifndef I_SINGLETON
#define I_SINGLETON(Class) friend class ISingleton<Class>;
#endif

template<class T>
class ISingleton
{
protected:
    ISingleton() { }
    ~ISingleton() { }

    static T* m_instance;
    static int m_ref_count;

public:
    /* Методы без подсчета ссылок */
    inline static T* instance() {
        if( !m_instance || m_instance == nullptr ) {
            m_instance = new T;
            m_ref_count = 1;
        }

        return m_instance;
    }

    inline static T& inst() {
        if( !m_instance || m_instance == nullptr ) {
            m_instance = new T;
            m_ref_count = 1;
        }

        return *m_instance;
    }

    inline static T* getInstance() {
        return( instance() );
    }

    /* Методы с подсчетом ссылок */
    inline static T* instanceSafe() {
        if( m_instance == nullptr ) m_instance = new T;
        ++m_ref_count;

        return m_instance;
    }

    inline static T* getInstanceSafe() {
        return( instanceSafe() );
    }

    inline static void free() {
        if( m_ref_count <= 0 ) delete m_instance;
        else --m_ref_count;
    }

    inline static void freeInstance() {
        free();
    }

    inline static void deleteInstance() {
        if( m_instance != nullptr && m_ref_count <= 1 ) {
            delete m_instance;
            m_instance = nullptr;
            m_ref_count = 0;
        }
    }
};

template<class T>
T*  ISingleton<T>::m_instance = nullptr;

template<class T>
int  ISingleton<T>::m_ref_count = 0;

#endif // INTERFACE_SINGLETON_H
