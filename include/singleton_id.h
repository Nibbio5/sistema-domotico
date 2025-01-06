// Eros Menin

#ifndef SINGLETONID_H
#define SINGLETONID_H

/**
 * @brief Class used to manage the creation of unique numeric IDs.
 * 
 * By using the Singeton design pattern, this class can provide the same instance 
 * every time it's called, without the need to be instantiated.
 * 
 * The IDs are generated starting from zero and incrementing a variable on every call.
 * To get the ID the call is: ``SingeltonId::getInstance().generateId()``
 * 
 */
class SingletonId {
    public:
        /**
         * @brief Uses the Singleton design pattern to always provide the same instance
         * of SingletonId object.
         * 
         * @return SingeltonId& the instance of SingletonId
         */
        static SingletonId& getInstance() {
            static SingletonId instance;
            return instance;
        }

        /**
         * @brief Returns the new ID and then increments the internal variable.
         * 
         * @return int a new ID
         */
        int generateId();

    private:
        /**
         * @brief Stores the last generated ID
         * 
         */
        int current_id_;

        /**
         * @brief Construct a new SingeltonId object.
         * 
         * The constructor is private to avoid direct instantiation.
         * 
         */
        SingletonId();

        /**
         * @brief Copy construct disabled to avoid copy.
         * 
         */
        SingletonId(const SingletonId&) = delete;

        /**
         * @brief Copy assignment disabled to avoid copy.
         * 
         * @return SingeltonId& the copied SingeltonId object
         */
        SingletonId& operator=(const SingletonId&) = delete;
};

#endif