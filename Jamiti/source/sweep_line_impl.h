#pragma once
#include "..\include\sweep_line.h"

namespace jamiti
{
	class SweeplineEvent
	{
	public:
		enum class Type
		{
			From = 0,
			To,
			Intersection
		};

		static SweeplineEvent ConstructFromEvent(size_t segment_index, const Point& event_point) {
			return SweeplineEvent(segment_index, -1, event_point, Type::From);
		}

		static SweeplineEvent ConstructToEvent(size_t segment_index, const Point& event_point) {
			return SweeplineEvent(segment_index, -1, event_point, Type::To);
		}

		static SweeplineEvent ConstructIntersectionEvent(size_t segment_index_1, size_t segment_index_2, const Point& event_point) {
			return SweeplineEvent(segment_index_1, segment_index_2, event_point, Type::Intersection);
		}

	public:
		SweeplineEvent(size_t index_1, size_t index_2, const Point& pt, Type event_type)
			:m_index_1(index_1),
			m_index_2(index_2),
			m_point(pt),
			m_event_type(event_type)
		{
		}

		size_t m_index_1;
		size_t m_index_2;
		Point m_point;
		Type m_event_type;

		friend struct Event_comparator;
	};

	//may be we can sort events by event type too.
	struct Event_comparator
	{
		bool operator()(const SweeplineEvent& event_1, const SweeplineEvent& event_2) const
		{
			if (event_1.m_point.m_x < event_2.m_point.m_x)
			{
				return true;
			}
			else
			{
				if (event_1.m_point.m_x == event_2.m_point.m_x)
				{
					if (event_1.m_point.m_y == event_2.m_point.m_y)
					{
						return event_1.m_event_type < event_2.m_event_type;
					}
					else
					{
						return event_1.m_point.m_y < event_2.m_point.m_y;
					}
				}
			}
			return false;
		}
	};

	class SweepLineImpl;
	struct SweepLineStatusComparator
	{
		SweepLineImpl* m_sweep_line_impl;
		SweepLineStatusComparator(SweepLineImpl* sweep_line_impl);
		bool operator()(size_t seg_1_index, size_t seg_2_index) const;
		bool operator()(size_t seg_1_index, size_t seg_2_index);
	};

	class SweepLineImpl
	{
	public:
		SweepLineImpl();
		~SweepLineImpl();

		void load_geometry(std::vector<Line> segments);
		void handle_event(const SweeplineEvent& sweep_event);
		void execute();

	private:

		auto find_segment_in_sweep_status(size_t index);

		std::vector<Line> m_segments;
		//maps event to segments whose left end point is event point
		std::multiset<SweeplineEvent, Event_comparator> m_event_queue;
		std::unordered_map<size_t, bool> m_segments_reoriented;
		std::multiset<size_t, SweepLineStatusComparator> m_sweep_status;
		double m_event_x;
		friend SweepLineStatusComparator;
	};
}