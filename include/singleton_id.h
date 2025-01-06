// Eros Menin

#ifndef SINGLETONID_H
#define SINGLETONID_H

/**
 * @brief Class used to manage the creation of unique numeric IDs.
 *
 * By using the Singeton design pattern, this class can provide the same
 * instance every time it's called, without the need to be instantiated.
 *
 * The IDs are generated starting from zero and incrementing a variable on every
 * call. To get the ID the call is: ``SingeltonId::getInstance().generateId()``
 *
 */
class SingeltonId {
public:
  /**
   * @brief Uses the Singleton design pattern to always provide the same
   * instance of SingletonId object.
   *
   * @return SingeltonId& the instance of SingletonId
   */
  static SingeltonId &getInstance() {
    static SingeltonId instance;
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
  int currentId;

  /**
   * @brief Construct a new SingeltonId object.
   *
   * The constructor is private to avoid direct instantiation.
   *
   */
  SingeltonId();

  /**
   * @brief Copy construct disabled to avoid copy.
   *
   */
  SingeltonId(const SingeltonId &) = delete;

  /**
   * @brief Copy assignment disabled to avoid copy.
   *
   * @return SingeltonId& the copied SingeltonId object
   */
  SingeltonId &operator=(const SingeltonId &) = delete;
};

#endif
