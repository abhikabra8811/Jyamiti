#include "sweep_line_impl.h"

namespace
{
	//return true if swapped
	bool swap_lexographical(jamiti::Point& p1, jamiti::Point& p2)
	{
		if (p1.m_x < p2.m_x || ((p1.m_x == p2.m_x) && (p1.m_y < p2.m_y)))
		{
			return false;
		}

		std::swap(p1, p2);
		return true;
	}
}

namespace jamiti 
{
	SweepLineImpl::SweepLineImpl()
		:m_sweep_status(SweepLineStatusComparator(this))
	{
	}

	SweepLineImpl::~SweepLineImpl()
	{
	}

	void SweepLineImpl::load_geometry(std::vector<Line> segments)
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
	}

	auto SweepLineImpl::find_segment_in_sweep_status(size_t index)
	{
		auto iter_start = std::lower_bound(std::begin(m_sweep_status), std::end(m_sweep_status), index, m_sweep_status.key_comp());
 		auto iter_end = std::upper_bound(std::begin(m_sweep_status), std::end(m_sweep_status), index, m_sweep_status.key_comp());
		while (iter_start != iter_end)
		{
			if (*iter_start == index)
				return iter_start;
			++iter_start;
		}
		return std::end(m_sweep_status);
	}

	void SweepLineImpl::handle_event(const SweeplineEvent& sweep_event)
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
				const Line& next_seg = m_segments[*iter_next];
				std::vector<Point> intersections = this_seg.intersect(next_seg);
				if (intersections.size() != 0)
				{
					m_event_queue.insert(SweeplineEvent::ConstructIntersectionEvent(*iter, *iter_next, intersections[0]));
				}
			}

			if (iter != std::begin(m_sweep_status))
			{
				auto iter_prev = std::prev(iter);
				const Line& prev_seg = m_segments[*iter_prev];
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
			auto iter = find_segment_in_sweep_status(sweep_event.m_index_1);
			if (iter != m_sweep_status.begin() && iter != std::prev(m_sweep_status.end()))
			{
				auto iter_prev = std::prev(iter);
				auto iter_next = std::next(iter);
				const Line& prev_seg = m_segments[*iter_prev];
				const Line& next_seg = m_segments[*iter_next];
				std::vector<Point> intersections = prev_seg.intersect(next_seg);
				if (intersections.size() != 0 && m_event_x <= intersections[0].m_x)
				{
					m_event_queue.insert(SweeplineEvent::ConstructIntersectionEvent(*iter_prev, *iter_next, intersections[0]));
				}
				m_sweep_status.erase(iter);
			}
			break;
		}
		case SweeplineEvent::Type::Intersection:
		{
			m_intersections.push_back({ sweep_event.m_point, sweep_event.m_index_1, sweep_event.m_index_2 });
			auto iter_1 = find_segment_in_sweep_status(sweep_event.m_index_1);
			auto iter_2 = find_segment_in_sweep_status(sweep_event.m_index_2);
			const Line& line_1 = m_segments[*iter_1];
			const Line& line_2 = m_segments[*iter_2];
			double y_1 = !m_segments_reoriented[*iter_1] ? line_1.m_start_pt.m_y : line_1.m_end_pt.m_y;
			double y_2 = !m_segments_reoriented[*iter_2] ? line_2.m_start_pt.m_y : line_2.m_end_pt.m_y;

			if (y_1 < y_2)
			{
				std::swap(iter_1, iter_2);
			}

			auto iter_next = std::next(iter_1);
			if (iter_next != m_sweep_status.end())
			{
				std::vector<Point> intersections = m_segments[*iter_2].intersect(m_segments[*iter_next]);
				if (intersections.size() != 0)
				{
					m_event_queue.insert(SweeplineEvent::ConstructIntersectionEvent(*iter_2, *iter_next, intersections[0]));
				}
			}

			if (iter_2 != m_sweep_status.begin())
			{
				auto iter_prev = std::prev(iter_2);
				std::vector<Point> intersections = m_segments[*iter_1].intersect(m_segments[*iter_prev]);
				if (intersections.size() != 0)
				{
					m_event_queue.insert(SweeplineEvent::ConstructIntersectionEvent(*iter_1, *iter_prev, intersections[0]));
				}
			}

			auto i1 = *iter_1;
			auto i2 = *iter_2;
			m_sweep_status.erase(iter_1);
			m_sweep_status.erase(iter_2);

			m_sweep_status.insert(i1);
			m_sweep_status.insert(i2);
			break;
		}
		default:
			break;
		}
	}

	std::vector<SweepLine::Intersection> SweepLineImpl::get_intersectoion() const
	{
		return m_intersections;
	}

	void SweepLineImpl::execute()
	{
		while(!m_event_queue.empty())
		{
			auto sweep_event_iter = m_event_queue.begin();
			auto sweep_event = *sweep_event_iter;
			m_event_queue.erase(sweep_event_iter);
			this->m_event_x = sweep_event.m_point.m_x;
			handle_event(sweep_event);
		}
	}

	SweepLineStatusComparator::SweepLineStatusComparator(SweepLineImpl* sweep_line_impl)
		:m_sweep_line_impl(sweep_line_impl)
	{
	}

	bool SweepLineStatusComparator::operator()(size_t seg_1_index, size_t seg_2_index) const
	{
		const auto& seg_1 = (m_sweep_line_impl->m_segments)[seg_1_index];
		const auto& seg_2 = (m_sweep_line_impl->m_segments)[seg_2_index];
		return seg_1.get_y_at_x(m_sweep_line_impl->m_event_x) < seg_2.get_y_at_x(m_sweep_line_impl->m_event_x);
	}

	bool SweepLineStatusComparator::operator()(size_t seg_1_index, size_t seg_2_index)
	{
		const auto& seg_1 = (m_sweep_line_impl->m_segments)[seg_1_index];
		const auto& seg_2 = (m_sweep_line_impl->m_segments)[seg_2_index];
		return seg_1.get_y_at_x(m_sweep_line_impl->m_event_x) < seg_2.get_y_at_x(m_sweep_line_impl->m_event_x);
	}

} //namespace jamiti 
