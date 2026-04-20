#pragma once

#include <vector>
#include <memory>

struct EntityManager {

    static constexpr std::size_t MAX_ENTITIES { 100 };
    inline static std::size_t nextID { 0 };

    struct Entity {
        Entity() { m_id = nextID++; }

    private:
        std::size_t m_id {};
    };

};
/*
private:
    struct Entity {
        int id;
        RenderComponent* renderComponent;
        
        Entity(int id) : id(id), renderComponent(nullptr) {}
        
        void addRenderComponent(Vector2D pos, const char* texto, Window* w) {
            renderComponent = new RenderComponent(pos, texto, w);
        }
    };

    std::vector<std::unique_ptr<Entity>> entities;
    int nextID = 0;

public:

    Entity* createEntity() {
        entities.push_back(std::make_unique<Entity>(nextID++));
        return entities.back().get();
    }

    const std::vector<std::unique_ptr<Entity>>& getEntities() const {
        return entities;
    }

};

// Clase Entity
        struct Entity
        {
            friend struct EntityManager<CMPList, SNGCMPLIST, TAGList, SlotCapacity>;
            // Sacamos TypeList para asociar cada componente con su key
            using keytype_list = MP::forall_insert_template_t<to_keytype, CMPList>;
            // Tupla de los tipos de las keys de los componentes
            using key_storage_type = tuple_replace<keytype_list>;

            Entity() { id_ = nextID++; }

            // Plantilla para saber si la entidad tiene un componente del tipo CMP
            template <typename CMP>
            bool hasComponent() const noexcept
            {
                // Devuelve true si la máscara de componentes tiene el bit correspondiente al componente CMP
                return cmp_mask_ & cmp_info::template mask<CMP>();
            }

            // Plantilla para saber si la entidad tiene un tag del tipo TAG
            template <typename TAG>
            bool hasTag() const noexcept
            {
                // Devuelve true si la máscara de tags tiene el bit correspondiente al tag TAG
                return tag_mask_ & tag_info::template mask<TAG>();
            }

            // Plantilla para saber si la entidad tiene alguno de los tags TAGs
            template <typename... TAGs>
            bool hasTags(MP::TypeList<TAGs...>) const noexcept
            {
                // Devuelve true si la máscara de tags tiene el bit correspondiente a cualquiera de los tags TAGs
                return (... || (hasTag<TAGs>()));
            }

            std::size_t  getID() const noexcept { return id_; }

        private:
            // Plantilla para obtener la clave de un componente del tipo CMP
            template <typename CMP>
            to_keytype<CMP> getComponentKey() const
            {
                assert(hasComponent<CMP>());
                return std::get< to_keytype<CMP> >(cmp_keys_);
            }

            // Resetea la entidad
            void reset() noexcept
            {
                cmp_mask_ = 0;
                tag_mask_ = 0;
                cmp_keys_ = {};
            }

            // Asigna un nuevo ID a la entidad
            void setID(std::size_t const id) noexcept { this->id_ = id; }

            // Plantilla para añadir un componente a la entidad
            template <typename CMP>
            void addComponent(to_keytype<CMP> key)
            {
                // Actualiza la máscara de componentes para indicar que esta entidad ahora tiene un componente del tipo CMP
                cmp_mask_ |= cmp_info::template mask<CMP>();

                // Almacena la clave para el componente del tipo CMP
                std::get< to_keytype<CMP> >(cmp_keys_) = key;
            }

            // Plantilla para añadir un tag a la entidad
            template <typename TAG>
            void addTag()
            {
                // Se actualiza la máscara de tags para indicar que esta entidad ahora tiene un tag del tipo TAG
                tag_mask_ |= tag_info::template mask<TAG>();
            }

            std::size_t id_{}; // ID de la entidad
            typename cmp_info::mask_type cmp_mask_{}; // Máscara de componentes
            typename tag_info::mask_type tag_mask_{}; // Máscara de tags
            key_storage_type cmp_keys_{}; // Tupla de las claves de los componentes
        };
*/