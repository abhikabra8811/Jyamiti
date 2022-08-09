#include "..\include\line.h"
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

		friend class Event_comparator;
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

	//return true if swapped
	bool swap_lexographical(Point& p1, Point& p2)
	{
		if (p1.m_x < p2.m_x || ((p1.m_x == p2.m_x) && (p1.m_y < p2.m_y)))
		{
			return false;
		}

		std::swap(p1, p2);
		return true;
	}

	class SweepLine::Impl;
	struct SweepLineStatusComparator
	{
		const std::vector<Line>* m_segments = nullptr;
		double m_event_x;
		SweepLineStatusComparator() = default;
		SweepLineStatusComparator(const std::vector<Line>& sweep_line_impl)
			:m_segments(&sweep_line_impl)
		{
		}

		void SetSegments(const std::vector<Line>* vec)
		{
			m_segments = vec;
		}

		bool operator()(size_t seg_1_index, size_t seg_2_index) const
		{
			const auto& seg_1 = (*m_segments)[seg_1_index];
			const auto& seg_2 = (*m_segments)[seg_2_index];
			return seg_1.get_y_at_x(m_event_x) <= seg_2.get_y_at_x(m_event_x);
		}
	};

	class SweepLine::Impl
	{
	public:
		Impl()
			:m_sweep_status(SweepLineStatusComparator(m_segments))
		{
		}

		void load_geometry(std::vector<Line> segments)
		{
			m_segments = std::move(segments);
			for (size_t i = 0; i < m_segments.size(); ++i)
			{
				Point p1 = m_segments[i].m_start_pt;
				Point p2 = m_segments[i].m_end_pt;
				const bool swapped = swap_lexographical(p1, p2);
				m_event_queue.insert(SweeplineEvent::ConstructFromEvent(i, p1));
				m_event_queue.insert(SweeplineEvent::ConstructToEvent(i, p2));
				m_segments_reoriented[i] = swapped;
			}

			//m_sweep_status_comparator.SetSegments(&m_segments);
		}

		void handle_event(const SweeplineEvent& sweep_event)
		{
			auto event_point = sweep_event.m_point;
			switch (sweep_event.m_event_type)
			{
			case SweeplineEvent::Type::From:
			{
				auto iter = m_sweep_status.insert(sweep_event.m_index_1);
				const Line& this_seg = m_segments[sweep_event.m_index_1];
				//if (iter == m_sweep_status.begin())
				//{
					auto iter_next = std::next(iter);
					if (iter_next != std::end(m_sweep_status))
					{
						const Line& next_seg = m_segments[*std::next(iter)];
						std::vector<Point> intersections = this_seg.intersect(next_seg);
						if (intersections.size() != 0)
						{
							m_event_queue.insert(SweeplineEvent::ConstructIntersectionEvent(*iter, *iter_next, intersections[0]));
						}
					}
					
					if (iter != std::begin(m_sweep_status))
					{
						auto iter_prev = std::prev(iter);
						const Line& prev_seg = m_segments[*std::next(iter_prev)];
						std::vector<Point> intersections = this_seg.intersect(prev_seg);
						if (intersections.size() != 0)
						{
							m_event_queue.insert(SweeplineEvent::ConstructIntersectionEvent(*iter, *iter_prev, intersections[0]));
						}
					}
			
				//}
				//else if (iter == std::prev(m_sweep_status.end()))
				//{
				//	const Line& this_seg = m_segments[*iter];
				//	const Line& next_seg = m_segments[*(std::prev(iter))];
				//	std::vector<Point> intersections = this_seg.intersect(next_seg);
				//	if (intersections.size() != 0)
				//	{
				//		m_event_queue.insert(SweeplineEvent::ConstructIntersectionEvent(*(std::prev(iter)), *iter, intersections[0]));
				//	}
				//}
				//else
				//{
				//	const Line& this_seg = m_segments[*iter];
				//	const Line& next_seg = m_segments[*(std::next(iter))];
				//	std::vector<Point> intersections = this_seg.intersect(next_seg);
				//	if (intersections.size() != 0)
				//	{
				//		m_event_queue.insert(SweeplineEvent::ConstructIntersectionEvent(*iter, *std::next(iter), intersections[0]));
				//	}
				//	const Line& prev_seg = m_segments[*std::prev(iter)];
				//	intersections = this_seg.intersect(prev_seg);
				//	if (intersections.size() != 0)
				//	{
				//		m_event_queue.insert(SweeplineEvent::ConstructIntersectionEvent(*iter, *std::prev(iter), intersections[0]));
				//	}
				//}
					break;
			}
			case SweeplineEvent::Type::To:
			{
				//remove SweepStatusEvent
				auto iter = m_sweep_status.find(sweep_event.m_index_1);
				if (iter != m_sweep_status.begin() && iter != std::prev(m_sweep_status.end()))
				{
					auto iter_prev = std::prev(iter);
					auto iter_next = std::next(iter);
					const Line& prev_seg = m_segments[*iter_prev];
					const Line& next_seg = m_segments[*iter_next];
					std::vector<Point> intersections = prev_seg.intersect(next_seg);
					if (intersections.size() != 0)
					{
						m_event_queue.insert(SweeplineEvent::ConstructIntersectionEvent(*iter_prev, *iter_next, intersections[0]));
					}
				}
				break;
			}
			case SweeplineEvent::Type::Intersection:
			{
				auto iter_1 = m_sweep_status.find(sweep_event.m_index_1);
				auto iter_2 = m_sweep_status.find(sweep_event.m_index_2);
				const Line& line_1 = m_segments[*iter_1];
				const Line& line_2 = m_segments[*iter_2];
				double y_1 = !m_segments_reoriented[*iter_1] ? line_1.m_start_pt.m_y : line_1.m_end_pt.m_y;
				double y_2 = !m_segments_reoriented[*iter_2] ? line_2.m_start_pt.m_y : line_2.m_end_pt.m_y;

				if (y_1 < y_2)
				{
					std::swap(iter_1, iter_2);
				}

				auto iter_next = std::next(iter_1);
				auto iter_prev = std::prev(iter_2);				
				std::vector<Point> intersections = m_segments[*iter_2].intersect(m_segments[*iter_next]);
				if (intersections.size() != 0)
				{
					m_event_queue.insert(SweeplineEvent::ConstructIntersectionEvent(*iter_2, *iter_next, intersections[0]));
				}

				intersections = m_segments[*iter_1].intersect(m_segments[*iter_prev]);
				if (intersections.size() != 0)
				{
					m_event_queue.insert(SweeplineEvent::ConstructIntersectionEvent(*iter_1, *iter_prev, intersections[0]));
				}
				break;
			}
			default:
				break;
			}
		}

		void execute()
		{
			for (const auto& sweep_event : m_event_queue)
			{
				handle_event(sweep_event);
			}
		}

	private:
		std::vector<Line> m_segments;
		//maps event to segments whose left end point is event point
		std::multiset<SweeplineEvent, Event_comparator> m_event_queue;
		std::unordered_map<size_t, bool> m_segments_reoriented;
		std::multiset<size_t, SweepLineStatusComparator> m_sweep_status;
		SweepLineStatusComparator m_sweep_status_comparator;
	};


	SweepLine::SweepLine()
		:m_impl(std::make_unique<SweepLine::Impl>())
	{
	}
	SweepLine::~SweepLine()
	{
	}

	void SweepLine::load_geometry(std::vector<Line> segments)
	{
		m_impl->load_geometry(std::move(segments));
	}

	void SweepLine::execute()
	{
		m_impl->execute();
	}
} //namespace jamiti