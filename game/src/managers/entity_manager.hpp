#pragma once

#include <vector>
#include <memory>
#include <set>
#include <cassert>

#include  "../utils/meta_program.hpp"
#include  "../utils/slotmap.hpp"
#include "./utils/types.hpp"

template <typename CMPList, typename TAGList, std::size_t SlotCapacity = 350>
struct EntityManager {

    // Constants

    static constexpr std::size_t MAX_ENTITIES { 200 };
    inline static std::size_t nextID { 0 };

    // Forward declaration
    struct Entity;

    // Traits

    using cmp_info = MP::cmp_tag_traits<CMPList>;
    using tag_info = MP::cmp_tag_traits<TAGList>;
    using deathSet = std::set<std::size_t, std::greater<std::size_t>>;

    // Alias

    // CMP --> Slotmap<CMP, SlotCapacity>
    template <typename T>
    using to_slotmap = Slotmap<T, SlotCapacity>;

    // TypeList<A,B,C> --> tuple<Slotmap<A>, Slotmap<B>, Slotmap<C>>
    using storage_type = MP::replace_t<std::tuple, MP::forall_insert_template_t<to_slotmap, CMPList>>;

    // CMP --> key_type de su Slotmap
    template <typename CMP>
    using to_keytype = typename Slotmap<CMP, SlotCapacity>::key_type;

    // Entity

    struct Entity {
        friend struct EntityManager<CMPList, TAGList, SlotCapacity>;

        // TypeList<key_A, key_B, ...> --> tuple<key_A, key_B, ...>
        using keytype_list     = MP::forall_insert_template_t<to_keytype, CMPList>;
        using key_storage_type = MP::replace_t<std::tuple, keytype_list>;

        Entity() { m_id = nextID++; }

        template <typename CMP>
        bool hasComponent() const noexcept {
            return m_cmp_mask & cmp_info::template mask<CMP>();
        }

        template <typename TAG>
        bool hasTag() const noexcept {
            return m_tag_mask & tag_info::template mask<TAG>();
        }

        std::size_t getID() const noexcept { return m_id; }

    private:
        template <typename CMP>
        to_keytype<CMP> getComponentKey() const {
            assert(hasComponent<CMP>());
            return std::get<to_keytype<CMP>>(m_cmp_keys);
        }

        template <typename CMP>
        void addComponent(to_keytype<CMP> key) {
            m_cmp_mask |= cmp_info::template mask<CMP>();
            std::get<to_keytype<CMP>>(m_cmp_keys) = key;
        }

        template <typename TAG>
        void addTag() {
            m_tag_mask |= tag_info::template mask<TAG>();
        }

        void reset() noexcept {
            m_cmp_mask = {};
            m_tag_mask = {};
            m_cmp_keys = {};
        }

        void setID(std::size_t id) noexcept { m_id = id; }

        std::size_t                  m_id       {};
        typename cmp_info::mask_type m_cmp_mask {};
        typename tag_info::mask_type m_tag_mask {};
        key_storage_type             m_cmp_keys {};
    };

    // Entities

    Entity& newEntity() noexcept {
        assert(m_alive < MAX_ENTITIES);
        return m_entities[m_alive++];
    }

    void destroyEntities(deathSet const& set){
        for(auto const& id : set) destroyEntity(id);
    }

    void destroyAll() {
        while (m_alive > 0) destroyEntity(0);
    }

    void destroyEntityByID(std::size_t id) {
        for (std::size_t i = 0; i < m_alive; ++i) {
            if (m_entities[i].getID() == id) {
                destroyEntity(i);
                return;
            }
        }
    }

    // Queries
    std::size_t aliveCount()   const noexcept { return m_alive; } 
    std::size_t freeEntities() const noexcept { return MAX_ENTITIES - m_alive; } 

    auto getEntities() {
        return std::span{ m_entities.begin(), m_entities.begin() + m_alive };
    }

    Entity* getEntityByID(std::size_t id) noexcept {
        for (auto& e : getEntities())
            if (e.getID() == id) return &e;
        return nullptr;
    }

    // Components

    // Add component, if already exist: return component
    template <typename CMP, typename... Args>
    CMP& addComponent(Entity& e, Args&&... args) {
        if (e.template hasComponent<CMP>())
            return getComponent<CMP>(e);
        return createComponent<CMP>(e, std::forward<Args>(args)...);
    }

    template <typename CMP>
    CMP& getComponent(Entity& e) {
        auto key = e.template getComponentKey<CMP>();
        return getCMPStorage<CMP>()[key];
    }

    template <typename CMP>
    const CMP& getComponent(const Entity& e) const {
        auto key = e.template getComponentKey<CMP>();
        return getCMPStorage<CMP>()[key];
    }

    template <typename CMP>
    void destroyComponent(Entity& e) {
        if (!e.template hasComponent<CMP>()) return;

        auto key = e.template getComponentKey<CMP>();
        getCMPStorage<CMP>().erase(key);
        
        e.m_cmp_mask &= static_cast<typename cmp_info::mask_type>(~cmp_info::template mask<CMP>());
    }

    template<typename TAG>
    void destroyTag(Entity& e) {
        if (e.template hasTag<TAG>())
            e.tag_mask_ &= ~tag_info::template mask<TAG>();
    }

    // Iterator

    // forEach: CMPs and TAGs | forEach<MP::TypeList<A,B>, MP::TypeList<TagX>>(lambda)
    template <typename CMPs, typename TAGs>
    void forEach(auto&& func) { forEachImpl(func, CMPs{}, TAGs{}); }

    // forEachAny
    template <typename CMPs, typename TAGs>
    void forEachAny(auto&& func) { forEachImplAny(func, CMPs{}, TAGs{}); }

private:

    // Storage

    template <typename CMP>
    auto& getCMPStorage() noexcept {
        return std::get<Slotmap<CMP, SlotCapacity>>(m_cmpStorage);
    }
    template <typename CMP>
    const auto& getCMPStorage() const noexcept {
        return std::get<Slotmap<CMP, SlotCapacity>>(m_cmpStorage);
    }

    // Intern creation
    template <typename CMP, typename... Args>
    CMP& createComponent(Entity& e, Args&&... args) {
        auto& storage = getCMPStorage<CMP>();
        auto  key     = storage.push_back(CMP{ std::forward<Args>(args)... });
        e.template addComponent<CMP>(key);
        return storage[key];
    }

    // Destroy

    void destroyEntity(std::size_t index) {
        assert(index < m_alive && m_alive > 0);
        auto& e = m_entities[index];
        destroyAllCMPs(e);
        e.reset();
        
        // Swap with the last
        e = m_entities[m_alive - 1];
        m_entities[m_alive - 1].reset();
        e.setID(index);
        --m_alive;
    }

    void destroyAllCMPs(Entity& e) {
        MP::replace_t<std::tuple, CMPList> dummy{};
        MP::for_each_in_tuple(dummy, [&](auto& cmpInstance) {
            using CMP = std::remove_reference_t<decltype(cmpInstance)>;
            destroyComponent<CMP>(e);
        });
    }

    // for-each

    template <typename... CMPs, typename... TAGs>
    void forEachImpl(auto&& func, MP::TypeList<CMPs...>, MP::TypeList<TAGs...>) {
        for (Entity& e : getEntities()) {
            bool hasCMPs = (true  && ... && e.template hasComponent<CMPs>());
            bool hasTAGs = (true  && ... && e.template hasTag<TAGs>());
            if (hasCMPs && hasTAGs)
                func(e, getComponent<CMPs>(e)...);
        }
    }

    template <typename... CMPs, typename... TAGs>
    void forEachImplAny(auto&& func, MP::TypeList<CMPs...>, MP::TypeList<TAGs...>) {
        for (Entity& e : getEntities()) {
            bool hasAnyCMPs = (false || ... || e.template hasComponent<CMPs>());
            bool hasAnyTAGs = (false || ... || e.template hasTag<TAGs>());
            if (hasAnyCMPs || hasAnyTAGs)
                func(e);
        }
    }

    // Data

    std::size_t                      m_alive      {};
    std::array<Entity, MAX_ENTITIES> m_entities   {};
    storage_type                     m_cmpStorage {};
};