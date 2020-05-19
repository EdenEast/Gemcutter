// Copyright (c) 2017 Emilian Cioca
#include "Jewel3D/Precompiled.h"
#include "Entity.h"
#include "Jewel3D/Application/Logging.h"
#include "Jewel3D/Entity/Hierarchy.h"
#include "Jewel3D/Entity/Name.h"
#include "Jewel3D/Rendering/Renderable.h"

#include <algorithm>

namespace Jwl
{
	namespace detail
	{
		std::unordered_map<unsigned, std::vector<Entity*>> entityIndex;
		std::unordered_map<unsigned, std::vector<ComponentBase*>> componentIndex;
	}

	ComponentBase::ComponentBase(Entity& _owner, unsigned _componentId)
		: owner(_owner)
		, componentId(_componentId)
	{
	}

	ComponentBase& ComponentBase::operator=(const ComponentBase& other)
	{
		ASSERT(componentId == other.componentId, "Assignment operator between incompatible components.");
		// The owner reference is left untouched.
		return *this;
	}

	bool ComponentBase::IsEnabled() const
	{
		return isEnabled && owner.IsEnabled();
	}

	bool ComponentBase::IsComponentEnabled() const
	{
		return isEnabled;
	}

	unsigned ComponentBase::GenerateID()
	{
		static unsigned counter = 1;
		return counter++;
	}

	Entity::Entity(std::string name)
	{
		Add<Name>(std::move(name));
	}

	Entity::Entity(const Transform& pose)
		: Transform(pose)
	{
	}

	Entity::~Entity()
	{
		RemoveAllComponents();
		RemoveAllTags();
	}

	Entity::Ptr Entity::MakeNewRoot()
	{
		auto entity = MakeNew();
		entity->Add<Hierarchy>();

		return entity;
	}

	Entity::Ptr Entity::MakeNewRoot(std::string name)
	{
		auto entity = MakeNew();
		entity->Add<Hierarchy>();
		entity->Add<Name>(std::move(name));

		return entity;
	}

	Entity::Ptr Entity::MakeNewRoot(const Transform& pose)
	{
		auto entity = MakeNew(pose);
		entity->Add<Hierarchy>();

		return entity;
	}

	Entity::Ptr Entity::Duplicate() const
	{
		auto newEntity = MakeNew();

		newEntity->Transform::operator=(*this);

		/* Copy all tags */
		for (auto tag : this->tags)
		{
			newEntity->Tag(tag);
		}

		/* Copy all components */
		// The Renderable is always copied first in order to have the Shader Variants updated from
		// any of the new components initializing.
		auto* renderable = this->Try<Renderable>();
		if (renderable)
		{
			static_cast<const ComponentBase*>(renderable)->Copy(*newEntity);
		}

		// Mirror all components to the other entity.
		for (auto comp : this->components)
		{
			// Material component has already been copied over.
			if (comp == renderable)
				continue;

			comp->Copy(*newEntity);
		}

		if (!this->IsEnabled())
		{
			newEntity->Disable();
		}

		return newEntity;
	}

	void Entity::CopyComponent(const ComponentBase& source)
	{
		source.Copy(*this);
	}

	mat4 Entity::GetWorldTransform() const
	{
		mat4 result;
		if (auto* hierarchy = Try<Hierarchy>())
		{
			result = hierarchy->GetWorldTransform();
		}
		else
		{
			result = mat4(rotation, position, scale);
		}

		return result;
	}

	quat Entity::GetWorldRotation() const
	{
		quat result;
		if (auto* hierarchy = Try<Hierarchy>())
		{
			result = hierarchy->GetWorldRotation();
		}
		else
		{
			result = rotation;
		}

		return result;
	}

	void Entity::LookAt(const vec3& pos, const vec3& target, const vec3& up)
	{
		// Passthrough to get around C++ name-hiding.
		Transform::LookAt(pos, target, up);
	}

	void Entity::LookAt(const Entity& target, const vec3& up)
	{
		ASSERT(&target != this, "Entity cannot look at itself.");

		vec3 targetPos;
		if (auto* targetHierarchy = target.Try<Hierarchy>())
		{
			targetPos = targetHierarchy->GetWorldTransform().GetTranslation();
		}
		else
		{
			targetPos = target.position;
		}

		// If we have a parent, we are not in world-space.
		// We must resolve for the target position in local-space.
		auto* hierarchy = Try<Hierarchy>();
		if (hierarchy && !hierarchy->IsRoot())
		{
			targetPos = vec3(hierarchy->GetWorldTransform() * vec4(targetPos, 1.0f));
		}

		LookAt(position, targetPos, up);
	}

	void Entity::RemoveAllComponents()
	{
		unsigned i = components.size();

		// This loop safely removes all components even if they remove others during destruction.
		while (i-- != 0)
		{
			if (i < components.size())
			{
				auto comp = components[i];
				components.erase(components.begin() + i);

				if (comp->IsEnabled())
				{
					Unindex(*comp);
				}

				delete comp;
			}
		}
	}

	void Entity::RemoveAllTags()
	{
		if (IsEnabled())
		{
			for (auto tag : tags)
			{
				UnindexTag(tag);
			}
		}

		tags.clear();
	}

	void Entity::Enable()
	{
		if (isEnabled)
		{
			return;
		}

		// Re-Index all active components.
		for (auto comp : components)
		{
			if (comp->IsComponentEnabled())
			{
				Index(*comp);
				comp->OnEnable();
			}
		}

		// Re-Index all tags.
		for (auto tag : tags)
		{
			IndexTag(tag);
		}

		isEnabled = true;
	}

	void Entity::Disable()
	{
		if (!isEnabled)
		{
			return;
		}

		// UnIndex all active components.
		for (auto comp : components)
		{
			if (comp->IsComponentEnabled())
			{
				Unindex(*comp);
				comp->OnDisable();
			}
		}

		// UnIndex all tags.
		for (auto tag : tags)
		{
			UnindexTag(tag);
		}

		isEnabled = false;
	}

	bool Entity::IsEnabled() const
	{
		return isEnabled;
	}

	void Entity::Tag(unsigned tagId)
	{
		if (IsEnabled())
		{
			IndexTag(tagId);
		}

		tags.push_back(tagId);
	}

	void Entity::RemoveTag(unsigned tagId)
	{
		auto itr = std::find(tags.begin(), tags.end(), tagId);
		if (itr == tags.end())
			return;

		*itr = tags.back();
		tags.pop_back();

		if (IsEnabled())
		{
			UnindexTag(tagId);
		}
	}

	void Entity::IndexTag(unsigned tagId)
	{
		// Adjust [id, entity] index.
		auto& table = detail::entityIndex[tagId];
		table.insert(std::lower_bound(table.begin(), table.end(), this), this);
	}

	void Entity::UnindexTag(unsigned tagId)
	{
		// Adjust [id, entity] index.
		auto& table = detail::entityIndex[tagId];
		table.erase(std::lower_bound(table.begin(), table.end(), this));
	}

	void Entity::Index(ComponentBase& comp)
	{
		// Adjust [id, entity] index.
		IndexTag(comp.componentId);

		// Adjust [id, component] index.
		detail::componentIndex[comp.componentId].push_back(&comp);
	}

	void Entity::Unindex(ComponentBase& comp)
	{
		// Adjust [id, entity] index.
		UnindexTag(comp.componentId);

		// Adjust [id, component] index.
		auto& componentTable = detail::componentIndex[comp.componentId];
		auto itr = std::find(componentTable.begin(), componentTable.end(), &comp);
		*itr = componentTable.back();
		componentTable.pop_back();
	}
}
