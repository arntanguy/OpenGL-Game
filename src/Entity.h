#ifndef __Entity__
#define __Entity__

#define OBJECT_NOT_COMPILED -1

/**
 * @brief An opengl entity.
 * This is a virtual class. If you want to create an entity, you must inherit this class
 * Typically, this would be a list of opengl commands,
 * usually used to build a model, either by using a model loader,
 * or by inheriting this virtual class and constructing it manually
 */
class Entity {
    public:
        Entity();
        virtual ~Entity();
        virtual void generate() = 0;
        virtual bool render() = 0;
};

#endif
